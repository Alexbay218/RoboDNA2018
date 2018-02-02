#include <random>
#include <math.h>
#include "match.h"

match::match(alliance red, alliance blue, int ti, int sd, int dw, sf::Font* f, int winX, int winY) {
    std::default_random_engine generator;
    generator.seed(sd);
    std::uniform_int_distribution<int> r1Y(3,8);
    std::uniform_int_distribution<int> r2Y(13,18);
    std::uniform_int_distribution<int> r3Y(19,23);
    std::uniform_int_distribution<int> b1Y(3,8);
    std::uniform_int_distribution<int> b2Y(9,13);
    std::uniform_int_distribution<int> b3Y(18,23);
    std::uniform_int_distribution<int> randConfig(0,1);
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

    config = "";
    for(int j = 0;j < 3;j++) {
        if(randConfig(generator)) {
            config += 't';
        }
        else {
            config += 'b';
        }
    }

    isRunning = true;
    if(dw > 0) {
        gameField = field(rtmp, btmp, dw);

    }
    else {
        gameField = field(rtmp, btmp, 1);
    }

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

    execMatch = std::thread([&](){
        clock.restart();
        int t = 0;
        int lastTimestamp = clock.getElapsedTime().asMilliseconds();
        int timeInterval = ti;
        while(t < 150) {
            update(t);
            while(timeInterval > 0 && clock.getElapsedTime().asMilliseconds() < lastTimestamp + timeInterval) {}
            lastTimestamp = clock.getElapsedTime().asMilliseconds();
            t++;
        }
        isRunning = false;
    });

    if(dw > 0) {
        winThread = std::thread([&](){
            sf::Context context;
            window.create(sf::VideoMode(
                static_cast<int>(gameField.backGround.getSize().x*1.125f),
                static_cast<int>(gameField.backGround.getSize().y*1.125f)
            ), "RoboDNA2018", sf::Style::Titlebar);
            window.setPosition(sf::Vector2i(static_cast<int>(window.getSize().x*winX), static_cast<int>(window.getSize().y*winY)));
            window.setActive(false);

            int i = 0;
            while(i < gameField.elements.size()) {
                gameField.elements[i].drawText.setFont(*f);
                i++;
            }
            i = 0;
            int j = 0;
            while(i < gameField.alliances.size()) {
                j = 0;
                while(j < gameField.alliances[i].robots.size()) {
                    gameField.alliances[i].robots[j].drawText.setFont(*f);
                    j++;
                }
                i++;
            }
            while(window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                }
                if(!isRunning) {
                    window.close();
                    return 0;
                }
                window.clear();
                gameField.draw(&window);
                window.display();
            }
        });
    }
}

match::~match() {
}

void match::update(int t) {
    //targeting
    int a = 0;
    int b = 0;
    while(a < gameField.alliances.size()) {
        b = 0;
        while(b < gameField.alliances[a].robots.size()) {
            char target = gameField.alliances[a].robots[b].targetAtTime(t);
            std::vector<int> tmpTarCoord;
            if(gameField.alliances[a].robots[b].hasCube || target == 'a') {
                tmpTarCoord = targetCoord(gameField.alliances[a].robots[b].posX, gameField.alliances[a].robots[b].posY, gameField.alliances[a].isRed, target);
            }
            else {
                tmpTarCoord = sourceCoord(gameField.alliances[a].robots[b].posX, gameField.alliances[a].robots[b].posY, gameField.alliances[a].isRed, gameField.alliances[a].robots[b].sourcePriority);
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
    int c = 0;
    int d = 0;
    bool collision = false;
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
            if(gameField.alliances[a].robots[b].tarX-gameField.alliances[a].robots[b].posX != 0) {
                angle = atan2((gameField.alliances[a].robots[b].tarY-gameField.alliances[a].robots[b].posY), (gameField.alliances[a].robots[b].tarX-gameField.alliances[a].robots[b].posX));
            }
            double mag = gameField.alliances[a].robots[b].curMaxVel;
            if(sqrt(pow(gameField.alliances[a].robots[b].tarX-gameField.alliances[a].robots[b].posX, 2) + pow(gameField.alliances[a].robots[b].tarY-gameField.alliances[a].robots[b].posY, 2)) < mag) {
                mag = sqrt(pow(gameField.alliances[a].robots[b].tarX-gameField.alliances[a].robots[b].posX, 2) + pow(gameField.alliances[a].robots[b].tarY-gameField.alliances[a].robots[b].posY, 2));
            }
            std::cout << "T: " << t << " Mag: " << mag << " X: " << gameField.alliances[a].robots[b].posX << " Y: " << gameField.alliances[a].robots[b].posY << std::endl;
            std::cout << "     TarX: " << gameField.alliances[a].robots[b].tarX << " TarY:" << gameField.alliances[a].robots[b].tarY << std::endl;
            std::cout << "     Angle: " << angle*57.2957795 << std::endl;
            std::cout << "     Has Cube: " << gameField.alliances[a].robots[b].hasCube << std::endl;
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
    //update dropping
    a = 0;
    b = 0;
    while(a < gameField.alliances.size()) {
        b = 0;
        while(b < gameField.alliances[a].robots.size()) {
            char target = gameField.alliances[a].robots[b].targetAtTime(t);
            if(gameField.alliances[a].robots[b].hasCube) {
                gameField.alliances[a].robots[b].hasCube = targetDrop(gameField.alliances[a].robots[b].posX, gameField.alliances[a].robots[b].posY, gameField.alliances[a].isRed, target);
            }
            else {
                gameField.alliances[a].robots[b].hasCube = sourceGrab(gameField.alliances[a].robots[b].posX, gameField.alliances[a].robots[b].posY, gameField.alliances[a].isRed, gameField.alliances[a].robots[b].sourcePriority);
            }
            b++;
        }
        a++;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------
std::vector<int> match::targetCoord(int rx, int ry, bool isRed, char tar) {
    int newX = 0;
    int newY = 0;
    std::string cconfig = config;
    //conversion to red only perspective
    std::vector<int> tmpCoord;
    int cx, cy;
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
                if(rx >= 12 && rx <= 15) {
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
    int newX = 0;
    int newY = 0;

    int i = sourcePriority.size();
    char tar = 'n';

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
    std::cout << "   Tar: " << tar << std::endl;
    //conversion to red only perspective
    std::vector<int> tmpCoord;
    int cx, cy;
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
    int i = sourcePriority.size();
    char tar = 'n';

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
