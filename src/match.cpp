#include "match.h"


match::match(alliance red, alliance blue, int ti, int sd, int dw, sf::Font* f, int winX, int winY) {
    generator.seed(sd);
    std::uniform_int_distribution<int> r1Y(3,8);
    std::uniform_int_distribution<int> r2Y(13,18);
    std::uniform_int_distribution<int> r3Y(19,23);
    std::uniform_int_distribution<int> b1Y(3,8);
    std::uniform_int_distribution<int> b2Y(9,13);
    std::uniform_int_distribution<int> b3Y(18,23);
    std::uniform_int_distribution<int> randConfig(0,1);
    climb = std::uniform_int_distribution<int>(1, 8); //12.5% chance for climb

    speedMulti = 1;

    alliance rtmp = red;
    alliance btmp = blue;
    rtmp.robots[0].posY = r1Y(generator);
    rtmp.robots[1].posY = r2Y(generator);
    rtmp.robots[2].posY = r3Y(generator);
    btmp.robots[0].posY = b1Y(generator);
    btmp.robots[1].posY = b2Y(generator);
    btmp.robots[2].posY = b3Y(generator);
    btmp.robots[0].posX = 53;
    btmp.robots[1].posX = 53;
    btmp.robots[2].posX = 53;

    isRunning = true;
    gameField = field(rtmp, btmp, dw);

    //set config (string representing the position of red position from left to right)
    config = "";
    for(int j = 0;j < 3;j++) {
        if(randConfig(generator) == 1) {
            config += 't';
        }
        else {
            config += 'b';
        }
    }

    if(randConfig(generator) == 1) {
        gameField.elements[0].currentCubeNum = 5;
    }
    else {
        gameField.elements[1].currentCubeNum = 5;
    }
    if(randConfig(generator) == 1) {
        gameField.elements[2].currentCubeNum = 5;
    }
    else {
        gameField.elements[3].currentCubeNum = 5;
    }


    execMatch = std::thread([&](){
        int t = 0;
        while(t < 150) {
            update(t);
            if(ti > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(ti));
            }
            t++;
        }
        isRunning = false;
        std::string res = ("Red: " + std::to_string(gameField.alliances[0].totalScore) +  " Blue: " + std::to_string(gameField.alliances[1].totalScore) + "\n");
        cout_mutex.lock();
        std::cout << res;
        cout_mutex.unlock();
    });

    if(dw > 0) {
        if(config[0] == 't') {
            gameField.elements[17].drawShape.setFillColor(sf::Color::Red);
            gameField.elements[18].drawShape.setFillColor(sf::Color::Blue);
        }
        else {
            gameField.elements[17].drawShape.setFillColor(sf::Color::Blue);
            gameField.elements[18].drawShape.setFillColor(sf::Color::Red);
        }
        if(config[1] == 't') {
            gameField.elements[21].drawShape.setFillColor(sf::Color::Red);
            gameField.elements[22].drawShape.setFillColor(sf::Color::Blue);
        }
        else {
            gameField.elements[21].drawShape.setFillColor(sf::Color::Blue);
            gameField.elements[22].drawShape.setFillColor(sf::Color::Red);
        }
        if(config[2] == 't') {
            gameField.elements[19].drawShape.setFillColor(sf::Color::Red);
            gameField.elements[20].drawShape.setFillColor(sf::Color::Blue);
        }
        else {
            gameField.elements[19].drawShape.setFillColor(sf::Color::Blue);
            gameField.elements[20].drawShape.setFillColor(sf::Color::Red);
        }
        winThread = std::thread([this, winX, winY, f, ti](){
            sf::Context context;

            this->window.create(sf::VideoMode(
                static_cast<int>(this->gameField.backGround.getSize().x*1.125f),
                static_cast<int>(this->gameField.backGround.getSize().y*1.125f)
            ), "RoboDNA2018", sf::Style::None);
            this->window.setPosition(sf::Vector2i(static_cast<int>(this->gameField.backGround.getSize().x*1.125f*winX), static_cast<int>(this->gameField.backGround.getSize().y*1.125f*winY)));
            this->window.setFramerateLimit(1000/ti);
            this->window.setActive(false);
            //set fonts
            this->gameField.redScore.setFont(*f);
            this->gameField.blueScore.setFont(*f);
            this->gameField.time.setFont(*f);

            int i = 0;
            while(i < this->gameField.elements.size()) {
                this->gameField.elements[i].drawText.setFont(*f);
                i++;
            }
            i = 0;
            int j = 0;
            while(i < this->gameField.alliances.size()) {
                j = 0;
                while(j < this->gameField.alliances[i].robots.size()) {
                    this->gameField.alliances[i].robots[j].drawText.setFont(*f);
                    j++;
                }
                i++;
            }
            while(this->window.isOpen()) {
                sf::Event event;
                while (this->window.pollEvent(event)) {
                }
                if(!this->isRunning) {
                    this->window.close();
                }
                this->window.clear();
                this->gameField.draw(&window);
                this->window.display();
            }
        });
    }
}

match::~match() {
}

void match::update(int t) {
    //update dropping
    a = 0;
    b = 0;
    while(a < gameField.alliances.size()) {
        b = 0;
        while(b < gameField.alliances[a].robots.size()) {
            char target = gameField.alliances[a].robots[b].targetAtTime(t);
            if(gameField.alliances[a].robots[b].hasCube) {
                gameField.alliances[a].robots[b].hasCube = targetDrop(gameField.alliances[a].robots[b].posX, gameField.alliances[a].robots[b].posY, gameField.alliances[a].isRed, target);
                if(!gameField.alliances[a].robots[b].hasCube) {
                    gameField.alliances[a].robots[b].timeOut = 3; //dropping delay
                }
            }
            else {
                gameField.alliances[a].robots[b].hasCube = sourceGrab(gameField.alliances[a].robots[b].posX, gameField.alliances[a].robots[b].posY, gameField.alliances[a].isRed, gameField.alliances[a].robots[b].sourcePriority);
                if(gameField.alliances[a].robots[b].hasCube) {
                    gameField.alliances[a].robots[b].timeOut = 4; //grabbing time delay
                }
            }
            b++;
        }
        a++;
    }
    //update powerUp
    gameField.updatePowUp(t);
    //targeting
    a = 0;
    b = 0;
    while(a < gameField.alliances.size()) {
        b = 0;
        while(b < gameField.alliances[a].robots.size()) {
            target = gameField.alliances[a].robots[b].targetAtTime(t);
            if(gameField.alliances[a].robots[b].timeOut > 0) {
                tmpTarCoord.clear();
                tmpTarCoord.push_back(gameField.alliances[a].robots[b].posX);
                tmpTarCoord.push_back(gameField.alliances[a].robots[b].posY);
                gameField.alliances[a].robots[b].timeOut--;
            }
            else if(gameField.alliances[a].robots[b].hasCube || target == 'a' || target == 'c') {
                tmpTarCoord = targetCoord(gameField.alliances[a].robots[b].posX, gameField.alliances[a].robots[b].posY, gameField.alliances[a].isRed, target);
                gameField.alliances[a].robots[b].timeOut = 0;
            }
            else {
                tmpTarCoord = sourceCoord(gameField.alliances[a].robots[b].posX, gameField.alliances[a].robots[b].posY, gameField.alliances[a].isRed, gameField.alliances[a].robots[b].sourcePriority);
                gameField.alliances[a].robots[b].timeOut = 0;
            }
            gameField.alliances[a].robots[b].tarX = tmpTarCoord[0];
            gameField.alliances[a].robots[b].tarY = tmpTarCoord[1];
            b++;
        }
        a++;
    }
    //vel slowdown
    a = 0;
    b = 0;
    c = 0;
    d = 0;
    collision = false;
    while(a < gameField.alliances.size()) {
        b = 0;
        while(b < gameField.alliances[a].robots.size()) {
            gameField.alliances[a].robots[b].curMaxVel = static_cast<int>(round(gameField.alliances[a].robots[b].maxVel*speedMulti));
            c = 0;
            d = 0;
            collision = false;
            while(c < gameField.alliances.size()) {
                while(d < gameField.alliances[c].robots.size()) {
                    if(c != a && b != d) {
                        if(abs(gameField.alliances[a].robots[b].posX - gameField.alliances[c].robots[d].posX) <= 3) {
                            if(abs(gameField.alliances[a].robots[b].posY - gameField.alliances[c].robots[d].posY) <= 3) {
                                collision = true;
                            }
                        }
                    }
                    d++;
                }
                c++;
            }
            if(collision) {
                gameField.alliances[a].robots[b].curMaxVel = static_cast<int>(round((gameField.alliances[a].robots[b].maxVel*speedMulti/2)));
            }
            b++;
        }
        a++;
    }
    //move bots
    a = 0;
    b = 0;
    while(a < gameField.alliances.size()) {
        b = 0;
        while(b < gameField.alliances[a].robots.size()) {
            double angle = 1.570796325;
            if(gameField.alliances[a].robots[b].tarX-gameField.alliances[a].robots[b].posX == 0 && gameField.alliances[a].robots[b].tarY-gameField.alliances[a].robots[b].posY < 0) {
                    angle = -angle;
            }
            if(gameField.alliances[a].robots[b].tarX-gameField.alliances[a].robots[b].posX != 0) {
                angle = atan2((gameField.alliances[a].robots[b].tarY-gameField.alliances[a].robots[b].posY), (gameField.alliances[a].robots[b].tarX-gameField.alliances[a].robots[b].posX));
            }
            double mag = gameField.alliances[a].robots[b].curMaxVel;
            if(sqrt(pow(gameField.alliances[a].robots[b].tarX-gameField.alliances[a].robots[b].posX, 2) + pow(gameField.alliances[a].robots[b].tarY-gameField.alliances[a].robots[b].posY, 2)) < mag) {
                mag = sqrt(pow(gameField.alliances[a].robots[b].tarX-gameField.alliances[a].robots[b].posX, 2) + pow(gameField.alliances[a].robots[b].tarY-gameField.alliances[a].robots[b].posY, 2));
            }
            /*
            std::cout << "T: " << t << " Mag: " << mag << " X: " << gameField.alliances[a].robots[b].posX << " Y: " << gameField.alliances[a].robots[b].posY << std::endl;
            std::cout << "     TarX: " << gameField.alliances[a].robots[b].tarX << " TarY:" << gameField.alliances[a].robots[b].tarY << std::endl;
            std::cout << "     Angle: " << angle*57.2957795 << std::endl;
            std::cout << "     Has Cube: " << gameField.alliances[a].robots[b].hasCube << std::endl;
            //*/
            gameField.alliances[a].robots[b].posY += static_cast<int>(round(sin(angle)*mag));
            gameField.alliances[a].robots[b].posX += static_cast<int>(round(cos(angle)*mag));
            if(gameField.alliances[a].robots[b].posY < 0) {
                gameField.alliances[a].robots[b].posY = 0;
            }
            if(gameField.alliances[a].robots[b].posY > 26) {
                gameField.alliances[a].robots[b].posY = 26;
            }
            if(gameField.alliances[a].robots[b].posX < 0) {
                gameField.alliances[a].robots[b].posX = 0;
            }
            if(gameField.alliances[a].robots[b].posX > 53) {
                gameField.alliances[a].robots[b].posX = 53;
            }
            b++;
        }
        a++;
    }
    //pass cubes to vault
    if(t == 149) {
        int tmpScore0 = gameField.elements[26].currentCubeNum;
        gameField.elements[26].currentCubeNum += gameField.elements[4].currentCubeNum;
        if(gameField.elements[26].currentCubeNum > 9) {
            gameField.elements[26].currentCubeNum = 9;
        }
        gameField.elements[4].currentCubeNum -= gameField.elements[26].currentCubeNum - tmpScore0;

        tmpScore0 = gameField.elements[27].currentCubeNum;
        gameField.elements[27].currentCubeNum += gameField.elements[5].currentCubeNum;
        if(gameField.elements[27].currentCubeNum > 9) {
            gameField.elements[27].currentCubeNum = 9;
        }
        gameField.elements[5].currentCubeNum -= gameField.elements[27].currentCubeNum - tmpScore0;
    }
    //update score
    updateScore(t);
    //update time
    gameField.time.setString("T = " + std::to_string(t));
}

void match::updateScore(int t) {
    if(t < 15) {
        i = 0;
        while(i < gameField.alliances[0].robots.size()) {
            if(gameField.alliances[0].robots[i].posX >= 10) {
                gameField.alliances[0].robots[i].hasAuto = true;
            }
            i++;
        }
        i = 0;
        while(i < gameField.alliances[1].robots.size()) {
            if(gameField.alliances[1].robots[i].posX <= 43) {
                gameField.alliances[1].robots[i].hasAuto = true;
            }
            i++;
        }
    }
    //red side switch
    if(gameField.alliances[0].powerUpCurrLvl[0] == 1 || gameField.alliances[0].powerUpCurrLvl[0] == 3) {
        gameField.alliances[0].cubeScore++;
    }
    else if(gameField.elements[17].currentCubeNum > gameField.elements[18].currentCubeNum) {
        if(config[0] == 't') {
            gameField.alliances[0].cubeScore++;
            if(t < 15) {
                gameField.alliances[0].cubeScore++;
                gameField.alliances[0].hasSwitchAuto = true;
            }
            if(gameField.alliances[0].powerUpCurrLvl[1] == 1 || gameField.alliances[0].powerUpCurrLvl[1] == 3) {
                gameField.alliances[0].cubeScore++;
            }
        }
    }
    else if(gameField.elements[17].currentCubeNum < gameField.elements[18].currentCubeNum) {
        if(config[0] == 'b') {
            gameField.alliances[0].cubeScore++;
            if(t < 15) {
                gameField.alliances[0].cubeScore++;
                gameField.alliances[0].hasSwitchAuto = true;
            }
            if(gameField.alliances[0].powerUpCurrLvl[1] == 1 || gameField.alliances[0].powerUpCurrLvl[1] == 3) {
                gameField.alliances[0].cubeScore++;
            }
        }
    }
    //scale
    if(gameField.alliances[0].powerUpCurrLvl[0] == 2 || gameField.alliances[0].powerUpCurrLvl[0] == 3) {
        gameField.alliances[0].cubeScore++;
    }
    else if(gameField.alliances[1].powerUpCurrLvl[0] == 2 || gameField.alliances[1].powerUpCurrLvl[0] == 3) {
        gameField.alliances[1].cubeScore++;
    }
    else if(gameField.elements[21].currentCubeNum > gameField.elements[22].currentCubeNum) {
        if(config[1] == 't') {
            gameField.alliances[0].cubeScore++;
            if(t < 15) {
                gameField.alliances[0].cubeScore++;
            }
            if(gameField.alliances[0].powerUpCurrLvl[1] == 1 || gameField.alliances[0].powerUpCurrLvl[1] == 3) {
                gameField.alliances[0].cubeScore++;
            }
        }
        else {
            gameField.alliances[1].cubeScore++;
            if(t < 15) {
                gameField.alliances[1].cubeScore++;
            }
            if(gameField.alliances[1].powerUpCurrLvl[1] == 1 || gameField.alliances[1].powerUpCurrLvl[1] == 3) {
                gameField.alliances[1].cubeScore++;
            }
        }
    }
    else if(gameField.elements[21].currentCubeNum < gameField.elements[22].currentCubeNum) {
        if(config[1] == 't') {
            gameField.alliances[1].cubeScore++;
            if(t < 15) {
                gameField.alliances[1].cubeScore++;
            }
            if(gameField.alliances[1].powerUpCurrLvl[1] == 1 || gameField.alliances[1].powerUpCurrLvl[1] == 3) {
                gameField.alliances[1].cubeScore++;
            }
        }
        else {
            gameField.alliances[0].cubeScore++;
            if(t < 15) {
                gameField.alliances[0].cubeScore++;
            }
            if(gameField.alliances[0].powerUpCurrLvl[1] == 1 || gameField.alliances[0].powerUpCurrLvl[1] == 3) {
                gameField.alliances[0].cubeScore++;
            }
        }
    }
    //blue side switch
    if(gameField.alliances[1].powerUpCurrLvl[0] == 1 || gameField.alliances[1].powerUpCurrLvl[0] == 3) {
        gameField.alliances[1].cubeScore++;
    }
    else if(gameField.elements[19].currentCubeNum > gameField.elements[20].currentCubeNum) {
        if(config[2] == 'b') {
            gameField.alliances[1].cubeScore++;
            if(t < 15) {
                gameField.alliances[1].cubeScore;
                gameField.alliances[1].hasSwitchAuto = true;
            }
            if(gameField.alliances[1].powerUpCurrLvl[1] == 1 || gameField.alliances[1].powerUpCurrLvl[1] == 3) {
                gameField.alliances[1].cubeScore++;
            }
        }
    }
    else if(gameField.elements[19].currentCubeNum < gameField.elements[20].currentCubeNum) {
        if(config[2] == 't') {
            gameField.alliances[1].cubeScore++;
            if(t < 15) {
                gameField.alliances[1].cubeScore;
                gameField.alliances[1].hasSwitchAuto = true;
            }
            if(gameField.alliances[1].powerUpCurrLvl[1] == 1 || gameField.alliances[1].powerUpCurrLvl[1] == 3) {
                gameField.alliances[1].cubeScore++;
            }
        }
    }

    //sum auto
    gameField.alliances[0].autoScore = 0;
    i = 0;
    while(i < gameField.alliances[0].robots.size()) {
        if(gameField.alliances[0].robots[i].hasAuto) {
            gameField.alliances[0].autoScore += 5;
        }
        i++;
    }
    gameField.alliances[1].autoScore = 0;
    i = 0;
    while(i < gameField.alliances[1].robots.size()) {
        if(gameField.alliances[1].robots[i].hasAuto) {
            gameField.alliances[1].autoScore += 5;
        }
        i++;
    }
    //sum vault
    gameField.alliances[0].vaultScore = gameField.elements[26].currentCubeNum*5;
    gameField.alliances[1].vaultScore = gameField.elements[27].currentCubeNum*5;
    //check and sum climb
    gameField.alliances[0].climbScore = 0;
    gameField.alliances[1].climbScore = 0;
    i = 0;
    while(i < gameField.alliances[0].robots.size()) {
        if(!gameField.alliances[0].robots[i].hasClimb && gameField.alliances[0].robots[i].posX >= 22 && gameField.alliances[0].robots[i].posX <= 25) {
            if(gameField.alliances[0].robots[i].targetAtTime(t) == 'c' && gameField.alliances[0].robots[i].posY >= 8 && gameField.alliances[0].robots[i].posY <= 18) {
                gameField.alliances[0].robots[i].hasClimb = (climb(generator) == 1);
            }
        }
        i++;
    }
    i = 0;
    while(i < gameField.alliances[1].robots.size()) {
        if(!gameField.alliances[1].robots[i].hasClimb && gameField.alliances[1].robots[i].posX >= 28 && gameField.alliances[1].robots[i].posX <= 31) {
            if(gameField.alliances[1].robots[i].targetAtTime(t) == 'c' && gameField.alliances[1].robots[i].posY >= 8 && gameField.alliances[1].robots[i].posY <= 18) {
                gameField.alliances[1].robots[i].hasClimb = (climb(generator) == 1);
            }
        }
        i++;
    }

    i = 0;
    while(i < gameField.alliances[0].robots.size()) {
        if(gameField.alliances[0].robots[i].hasClimb) {
            gameField.alliances[0].climbScore += 30;
        }
        i++;
    }
    if(gameField.alliances[0].powerUpCurrLvl[2] == 4 && gameField.alliances[0].climbScore < 90) {
        gameField.alliances[0].climbScore += 30;
    }
    i = 0;
    while(i < gameField.alliances[1].robots.size()) {
        if(gameField.alliances[1].robots[i].hasClimb) {
            gameField.alliances[1].climbScore += 30;
        }
        i++;
    }
    if(gameField.alliances[1].powerUpCurrLvl[2] == 4 && gameField.alliances[1].climbScore < 90) {
        gameField.alliances[1].climbScore += 30;
    }

    gameField.alliances[0].totalScore = gameField.alliances[0].autoScore + gameField.alliances[0].cubeScore + gameField.alliances[0].vaultScore + gameField.alliances[0].climbScore;
    gameField.alliances[1].totalScore = gameField.alliances[1].autoScore + gameField.alliances[1].cubeScore + gameField.alliances[1].vaultScore + gameField.alliances[1].climbScore;
    //gameField.alliances[0].totalScore = gameField.alliances[0].climbScore;
    //gameField.alliances[1].totalScore = gameField.alliances[1].climbScore;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<int> match::targetCoord(int rx, int ry, bool isRed, char tar) {
    newX = 0;
    newY = 0;
    cconfig = config;
    //conversion to red only perspective
    if(!isRed) {
        tmpCoord = gameField.coorConv(rx, ry);
        cx = tmpCoord[0];
        cy = tmpCoord[1];
        char tmpC = cconfig[0];
        cconfig[0] = cconfig[2];
        cconfig[2] = tmpC;
    }
    else {
        cx = rx;
        cy = ry;
    }
    //red path finding
    if(tar == 'e') {
        if(cx <= 12) {
            newX = 1;
            newY = 10;
        }
        if(cx > 12 && cx <= 26) {
            newX = 12;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 26 && cx <= 41) {
            newX = 26;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 41) {
            newX = 41;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
    }
    else if(tar == 'a') {
        if(cx < 10) {
            newX = 10;
            newY = cy;
        }
        else {
            newX = cx;
            newY = cy;
        }
    }
    else if(tar == 'w') {
        if(cx <= 26) {
            newX = 13;
            newY = 17;
            if(cconfig[0] == 't') {
                newY = 9;
            }
        }
        if(cx > 26 && cx <= 41) {
            newX = 26;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 41) {
            newX = 41;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
    }
    else if(tar == 'x') {
        if(cx < 12) {
            newX = 12;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx >= 12 && cx <= 26) {
            newX = 27;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 26) {
            newX = 40;
            newY = 17;
            if(cconfig[2] == 't') {
                newY = 9;
            }
        }
    }
    else if(tar == 's') {
        if(cx < 12) {
            newX = 12;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx >= 12 && cx <= 15) {
            newX = 16;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 15 && cx < 38) {
            newX = 26;
            newY = 19;
            if(cconfig[1] == 't') {
                newY = 7;
            }
        }
        if(cx >= 38 && cx <= 41) {
            newX = 37;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 41) {
            newX = 41;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
    }
    else if(tar == 'c') {
        if(cx < 12) {
            newX = 12;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx >= 12 && cx <= 15) {
            newX = 16;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 15 && cx < 25) {
            newX = 24;
            newY = 13;
        }
        if(cx >= 25 && cx <= 41) {
            newX = 24;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 41) {
            newX = 41;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
    }
    else {
        newX = cx;
        newY = cy;
    }
    //conversion back to red and blue
    tmpCoord.clear();
    if(!isRed) {
        tmpCoord = gameField.coorConv(newX, newY);
    }
    else {
        tmpCoord.push_back(newX);
        tmpCoord.push_back(newY);
    }
    return tmpCoord;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
bool match::targetDrop(int rx, int ry, bool isRed, char tar) {
    if(isRed) {
        if(tar == 'e' && gameField.elements[4].currentCubeNum < gameField.elements[4].maxCubeNum) {
            if(rx >= 0 && rx <= 2) {
                if(ry >= 9 && ry <= 12) {
                    gameField.elements[4].currentCubeNum++;
                    return false;
                }
            }
        }
        if(config[0] == 't') {
            if(tar == 'w' && gameField.elements[17].currentCubeNum < gameField.elements[17].maxCubeNum) {
                if(rx >= 12 && rx <= 15) {
                    if(ry >= 8 && ry <= 10) {
                        gameField.elements[17].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        else {
            if(tar == 'w' && gameField.elements[18].currentCubeNum < gameField.elements[18].maxCubeNum) {
                if(rx >= 12 && rx <= 15) {
                    if(ry >= 16 && ry <= 18) {
                        gameField.elements[18].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        if(config[2] == 't') {
            if(tar == 'x' && gameField.elements[19].currentCubeNum < gameField.elements[19].maxCubeNum) {
                if(rx >= 38 && rx <= 41) {
                    if(ry >= 8 && ry <= 10) {
                        gameField.elements[19].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        else {
            if(tar == 'x' && gameField.elements[20].currentCubeNum < gameField.elements[20].maxCubeNum) {
                if(rx >= 38 && rx <= 41) {
                    if(ry >= 16 && ry <= 18) {
                        gameField.elements[20].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        if(config[1] == 't') {
            if(tar == 's' && gameField.elements[21].currentCubeNum < gameField.elements[21].maxCubeNum) {
                if(rx >= 25 && rx <= 28) {
                    if(ry >= 6 && ry <= 8) {
                        gameField.elements[21].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        else {
            if(tar == 's' && gameField.elements[22].currentCubeNum < gameField.elements[22].maxCubeNum) {
                if(rx >= 25 && rx <= 28) {
                    if(ry >= 18 && ry <= 20) {
                        gameField.elements[22].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
    }
    else {
        if(tar == 'e' && gameField.elements[5].currentCubeNum < gameField.elements[5].maxCubeNum) {
            if(rx >= 51 && rx <= 53) {
                if(ry >= 14 && ry <= 17) {
                    gameField.elements[5].currentCubeNum++;
                    return false;
                }
            }
        }
        if(config[0] == 'b') {
            if(tar == 'x' && gameField.elements[17].currentCubeNum < gameField.elements[17].maxCubeNum) {
                if(rx >= 12 && rx <= 15) {
                    if(ry >= 8 && ry <= 10) {
                        gameField.elements[17].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        else {
            if(tar == 'x' && gameField.elements[18].currentCubeNum < gameField.elements[18].maxCubeNum) {
                if(rx >= 12 && rx <= 15) {
                    if(ry >= 16 && ry <= 18) {
                        gameField.elements[18].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        if(config[2] == 'b') {
            if(tar == 'w' && gameField.elements[19].currentCubeNum < gameField.elements[19].maxCubeNum) {
                if(rx >= 38 && rx <= 41) {
                    if(ry >= 8 && ry <= 10) {
                        gameField.elements[19].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        else {
            if(tar == 'w' && gameField.elements[20].currentCubeNum < gameField.elements[20].maxCubeNum) {
                if(rx >= 38 && rx <= 41) {
                    if(ry >= 16 && ry <= 18) {
                        gameField.elements[20].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        if(config[1] == 'b') {
            if(tar == 's' && gameField.elements[21].currentCubeNum < gameField.elements[21].maxCubeNum) {
                if(rx >= 25 && rx <= 28) {
                    if(ry >= 6 && ry <= 8) {
                        gameField.elements[21].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
        else {
            if(tar == 's' && gameField.elements[22].currentCubeNum < gameField.elements[22].maxCubeNum) {
                if(rx >= 25 && rx <= 28) {
                    if(ry >= 18 && ry <= 20) {
                        gameField.elements[22].currentCubeNum++;
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<int> match::sourceCoord(int rx, int ry, bool isRed, std::string sourcePriority) {
    newX = 0;
    newY = 0;

    i = sourcePriority.size();
    tar = 'n';

    if(isRed) {
        while(i >= 0) {
            if(sourcePriority[i] == 'e') {
                if(gameField.elements[4].currentCubeNum > 0) {
                    tar = 'e';
                }
            }
            if(sourcePriority[i] == 'z') {
                if(gameField.elements[8].currentCubeNum > 0) {
                    tar = 'z';
                }
            }
            if(sourcePriority[i] == 's') {
                if(gameField.elements[12].currentCubeNum > 0) {
                   tar = 's';
                }
            }
            if(sourcePriority[i] == 'o') {
                if(gameField.elements[13].currentCubeNum > 0) {
                   tar = 'o';
                }
            }
            if(sourcePriority[i] == 'p') {
                if(gameField.elements[2].currentCubeNum > 0) {
                   tar = 't';
                }
                if(gameField.elements[3].currentCubeNum > 0) {
                   tar = 'b';
                }
                if(gameField.elements[2].currentCubeNum > 0 && gameField.elements[3].currentCubeNum > 0) {
                    if(ry < 13) {
                        tar = 't';
                    }
                    else {
                        tar = 'b';
                    }
                }
            }
            i--;
        }
    }
    else {
        while(i >= 0) {
            if(sourcePriority[i] == 'e') {
                if(gameField.elements[5].currentCubeNum > 0) {
                    tar = 'e';
                }
            }
            if(sourcePriority[i] == 'z') {
                if(gameField.elements[9].currentCubeNum > 0) {
                    tar = 'z';
                }
            }
            if(sourcePriority[i] == 's') {
                if(gameField.elements[13].currentCubeNum > 0) {
                   tar = 's';
                }
            }
            if(sourcePriority[i] == 'o') {
                if(gameField.elements[12].currentCubeNum > 0) {
                   tar = 'o';
                }
            }
            if(sourcePriority[i] == 'p') {
                if(gameField.elements[0].currentCubeNum > 0) {
                   tar = 'b';
                }
                if(gameField.elements[1].currentCubeNum > 0) {
                   tar = 't';
                }
                if(gameField.elements[0].currentCubeNum > 0 && gameField.elements[1].currentCubeNum > 0) {
                    if(ry >= 13) {
                        tar = 't';
                    }
                    else {
                        tar = 'b';
                    }
                }
            }
            i--;
        }
    }
    //conversion to red only perspective
    tmpCoord.clear();
    if(!isRed) {
        tmpCoord = gameField.coorConv(rx, ry);
        cx = tmpCoord[0];
        cy = tmpCoord[1];
    }
    else {
        cx = rx;
        cy = ry;
    }
    //red path finding
    if(tar == 'n') {
        newX = cx;
        newY = cy;
    }
    if(tar == 'e') {
        if(cx <= 12) {
            newX = 1;
            newY = 10;
        }
        if(cx > 12 && cx <= 26) {
            newX = 12;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 26 && cx <= 41) {
            newX = 26;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 41) {
            newX = 41;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
    }
    if(tar == 'z') {
        if(cx <= 12) {
            newX = 10;
            newY = 13;
        }
        if(cx > 12 && cx <= 26) {
            newX = 12;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 26 && cx <= 41) {
            newX = 26;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 41) {
            newX = 41;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
    }
    if(tar == 's') {
        if(cx <= 12) {
            newX = 13;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 12 && cx <= 26) {
            newX = 17;
            newY = 13;
        }
        if(cx > 26 && cx <= 41) {
            newX = 26;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 41) {
            newX = 41;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
    }
    if(tar == 'o') {
        if(cx <= 12) {
            newX = 13;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 12 && cx <= 26) {
            newX = 27;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 26 && cx <= 41) {
            newX = 36;
            newY = 13;
        }
        if(cx > 41) {
            newX = 41;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
    }
    if(tar == 't') {
        if(cx <= 12) {
            newX = 13;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 12 && cx <= 26) {
            newX = 27;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 26 && cx <= 41) {
            newX = 42;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 41) {
            newX = 52;
            newY = 1;
        }
    }
    if(tar == 'b') {
        if(cx <= 12) {
            newX = 13;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 12 && cx <= 26) {
            newX = 27;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 26 && cx <= 41) {
            newX = 42;
            newY = 22;
            if(cy < 13) {
                newY = 4;
            }
        }
        if(cx > 41) {
            newX = 52;
            newY = 25;
        }
    }
    //conversion back to red and blue
    tmpCoord.clear();
    if(!isRed) {
        tmpCoord = gameField.coorConv(newX, newY);
    }
    else {
        tmpCoord.push_back(newX);
        tmpCoord.push_back(newY);
    }
    return tmpCoord;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
bool match::sourceGrab(int rx, int ry, bool isRed, std::string sourcePriority) {
    i = sourcePriority.size();
    tar = 'n';

    if(isRed) {
        while(i >= 0) {
            if(sourcePriority[i] == 'e') {
                if(gameField.elements[4].currentCubeNum > 0) {
                    tar = 'e';
                }
            }
            if(sourcePriority[i] == 'z') {
                if(gameField.elements[8].currentCubeNum > 0) {
                    tar = 'z';
                }
            }
            if(sourcePriority[i] == 's') {
                if(gameField.elements[12].currentCubeNum > 0) {
                   tar = 's';
                }
            }
            if(sourcePriority[i] == 'o') {
                if(gameField.elements[13].currentCubeNum > 0) {
                   tar = 'o';
                }
            }
            if(sourcePriority[i] == 'p') {
                if(gameField.elements[2].currentCubeNum > 0) {
                   tar = 't';
                }
                if(gameField.elements[3].currentCubeNum > 0) {
                   tar = 'b';
                }
                if(gameField.elements[2].currentCubeNum > 0 && gameField.elements[3].currentCubeNum > 0) {
                    if(ry < 13) {
                        tar = 't';
                    }
                    else {
                        tar = 'b';
                    }
                }
            }
            i--;
        }
    }
    else {
        while(i >= 0) {
            if(sourcePriority[i] == 'e') {
                if(gameField.elements[5].currentCubeNum > 0) {
                    tar = 'e';
                }
            }
            if(sourcePriority[i] == 'z') {
                if(gameField.elements[9].currentCubeNum > 0) {
                    tar = 'z';
                }
            }
            if(sourcePriority[i] == 's') {
                if(gameField.elements[13].currentCubeNum > 0) {
                   tar = 's';
                }
            }
            if(sourcePriority[i] == 'o') {
                if(gameField.elements[12].currentCubeNum > 0) {
                   tar = 'o';
                }
            }
            if(sourcePriority[i] == 'p') {
                if(gameField.elements[0].currentCubeNum > 0) {
                   tar = 't';
                }
                if(gameField.elements[1].currentCubeNum > 0) {
                   tar = 'b';
                }
                if(gameField.elements[0].currentCubeNum > 0 && gameField.elements[1].currentCubeNum > 0) {
                    if(ry < 13) {
                        tar = 't';
                    }
                    else {
                        tar = 'b';
                    }
                }
            }
            i--;
        }
    }
    if(isRed) {
        if(tar == 'e' && gameField.elements[4].currentCubeNum > 0) {
            if(rx >= 0 && rx <= 2) {
                if(ry >= 9 && ry <= 12) {
                    gameField.elements[4].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 'z' && gameField.elements[8].currentCubeNum > 0) {
            if(rx >= 9 && rx <= 12) {
                if(ry >= 12 && ry <= 14) {
                    gameField.elements[8].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 's' && gameField.elements[12].currentCubeNum > 0) {
            if(rx >= 16 && rx <= 17) {
                if(ry >= 9 && ry <= 17) {
                    gameField.elements[12].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 'o' && gameField.elements[13].currentCubeNum > 0) {
            if(rx >= 36 && rx <= 37) {
                if(ry >= 9 && ry <= 17) {
                    gameField.elements[13].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 't' && gameField.elements[2].currentCubeNum > 0) {
            if(rx >= 51 && rx <= 53) {
                if(ry >= 0 && ry <= 2) {
                    gameField.elements[2].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 'b' && gameField.elements[3].currentCubeNum > 0) {
            if(rx >= 51 && rx <= 53) {
                if(ry >= 24 && ry <= 26) {
                    gameField.elements[3].currentCubeNum--;
                    return true;
                }
            }
        }
    }
    else {
        if(tar == 'e' && gameField.elements[5].currentCubeNum > 0) {
            if(rx >= 51 && rx <= 53) {
                if(ry >= 14 && ry <= 17) {
                    gameField.elements[5].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 'z' && gameField.elements[9].currentCubeNum > 0) {
            if(rx >= 42 && rx <= 44) {
                if(ry >= 12 && ry <= 14) {
                    gameField.elements[9].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 's' && gameField.elements[13].currentCubeNum > 0) {
            if(rx >= 36 && rx <= 37) {
                if(ry >= 9 && ry <= 17) {
                    gameField.elements[13].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 'o' && gameField.elements[12].currentCubeNum > 0) {
            if(rx >= 16 && rx <= 17) {
                if(ry >= 9 && ry <= 17) {
                    gameField.elements[12].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 't' && gameField.elements[0].currentCubeNum > 0) {
            if(rx >= 0 && rx <= 2) {
                if(ry >= 0 && ry <= 2) {
                    gameField.elements[0].currentCubeNum--;
                    return true;
                }
            }
        }
        if(tar == 'b' && gameField.elements[1].currentCubeNum > 0) {
            if(rx >= 0 && rx <= 2) {
                if(ry >= 24 && ry <= 26) {
                    gameField.elements[1].currentCubeNum--;
                    return true;
                }
            }
        }
    }
    return false;
}
