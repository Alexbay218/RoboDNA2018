#include "field.h"
#include <math.h>

field::field() {
    std::vector<alliance> atmp;
    atmp.push_back(*(new alliance()));
    atmp.push_back(*(new alliance()));

    alliances = atmp;
    sizeMulti = 15;
    init();
}

field::field(alliance* red, alliance* blue, int sm, sf::Font* f) {
    alliances.clear();
    alliances.push_back(*red);
    alliances.push_back(*blue);
    alliances[0].isRed = true;
    alliances[1].isRed = false;

    sizeMulti = sm;
    init();
    redScore.setFont(*f);
    blueScore.setFont(*f);
    redPwrUp.setFont(*f);
    bluePwrUp.setFont(*f);
    time.setFont(*f);
    int i = 0;
    while(i < elements.size()) {
        elements[i].drawText.setFont(*f);
        i++;
    }
    i = 0;
    int j = 0;
    while(i < alliances.size()) {
        j = 0;
        while(j < alliances[i].robots.size()) {
            alliances[i].robots[j].drawText.setFont(*f);
            alliances[i].robots[j].drawText.setFont(*f);
            j++;
        }
        i++;
    }
}

field::~field() {;
}

void field::init() {
    backGround.setSize(sf::Vector2f(54*sizeMulti,27*sizeMulti));
    backGround.setFillColor(sf::Color(150,150,150));

    redScore.setCharacterSize(sizeMulti*2);
    blueScore.setCharacterSize(sizeMulti*2);
    redScore.setPosition((23*sizeMulti)-redScore.getLocalBounds().width, 27*sizeMulti);
    blueScore.setPosition(29*sizeMulti, 27*sizeMulti);

    redPwrUp.setCharacterSize(sizeMulti*1.75f);
    bluePwrUp.setCharacterSize(sizeMulti*1.75f);
    redPwrUp.setPosition((19*sizeMulti)-redScore.getLocalBounds().width, 27*sizeMulti);
    bluePwrUp.setPosition(33*sizeMulti, 27*sizeMulti);

    time.setCharacterSize(sizeMulti*1.8f);
    time.setPosition(54*sizeMulti, 27*sizeMulti);

    elements.clear();
    elements.push_back(element("topBluePortal",6,0, sf::Color::Blue,0,0,3,3,sizeMulti));         //00
    elements.push_back(element("botBluePortal",6,0, sf::Color::Blue,0,24,3,3,sizeMulti));        //01
    elements.push_back(element("topRedPortal",6,0, sf::Color::Red,51,0,3,3,sizeMulti));          //02
    elements.push_back(element("botRedPortal",6,0, sf::Color::Red,51,24,3,3,sizeMulti));         //03
    elements.push_back(element("redExchange",0,60, sf::Color::Red,0,9,3,4,sizeMulti));            //04
    elements.push_back(element("blueExchange",0,60, sf::Color::Blue,51,14,3,4,sizeMulti));        //05
    elements.push_back(element("redAutoLine",0,0, sf::Color(175,175,175),10,0,1,27,sizeMulti));  //06
    elements.push_back(element("blueAutoLine",0,0, sf::Color(175,175,175),43,0,1,27,sizeMulti)); //07
    elements.push_back(element("redPowCubeZone",10,0, sf::Color(200,0,0),9,12,3,3,sizeMulti));   //08
    elements.push_back(element("bluePowCubeZone",10,0, sf::Color(0,0,200),42,12,3,3,sizeMulti)); //09
    elements.push_back(element("redPlatformZone",0,0, sf::Color(200,0,0),22,8,4,11,sizeMulti));  //10
    elements.push_back(element("bluePlatformZone",0,0, sf::Color(0,0,200),28,8,4,11,sizeMulti)); //11
    elements.push_back(element("redBackZone",6,0, sf::Color(200,0,0),16,9,2,9,sizeMulti));       //12
    elements.push_back(element("blueBackZone",6,0, sf::Color(0,0,200),36,9,2,9,sizeMulti));      //13
    elements.push_back(element("redSwitchBlock",0,0, sf::Color(75,75,75),12,11,4,5,sizeMulti));  //14
    elements.push_back(element("blueSwitchBlock",0,0, sf::Color(75,75,75),38,11,4,5,sizeMulti)); //15
    elements.push_back(element("scaleBlock",0,0, sf::Color(75,75,75),26,9,2,9,sizeMulti));       //16
    elements.push_back(element("redTopSwitch",0,60, sf::Color::Yellow,12,8,4,3,sizeMulti));      //17
    elements.push_back(element("redBotSwitch",0,60, sf::Color::Yellow,12,16,4,3,sizeMulti));     //18
    elements.push_back(element("blueTopSwitch",0,60, sf::Color::Yellow,38,8,4,3,sizeMulti));     //19
    elements.push_back(element("blueBotSwitch",0,60, sf::Color::Yellow,38,16,4,3,sizeMulti));    //20
    elements.push_back(element("topScale",0,60, sf::Color::Yellow,25,6,4,3,sizeMulti));          //21
    elements.push_back(element("botScale",0,60, sf::Color::Yellow,25,18,4,3,sizeMulti));         //22
    elements.push_back(element("redScoreBG",0,0, sf::Color(235,0,0),0,27,27,7,sizeMulti));       //23
    elements.push_back(element("blueScoreBG",0,0, sf::Color(0,0,235),27,27,27,7,sizeMulti));     //24
    elements.push_back(element("sidePanelBG",0,0, sf::Color(200,200,200),54,0,7,34,sizeMulti));  //25
    elements.push_back(element("redVault",0,9, sf::Color::Red,55,7,5,5,sizeMulti));              //26
    elements.push_back(element("blueVault",0,9, sf::Color::Blue,55,15,5,5,sizeMulti));           //27

    int h = 0;
    int i = 0;
    while(h < alliances.size()) {
        i = 0;
        while(i < alliances[h].robots.size()) {
            alliances[h].robots[i].drawShape.setRadius(sizeMulti*(1/1.4142f));
            alliances[h].robots[i].drawShape.setOutlineThickness(sizeMulti/5);

            if(alliances[h].isRed) {
                alliances[h].robots[i].drawShape.setFillColor(sf::Color::Red);
            }
            else {
                alliances[h].robots[i].drawShape.setFillColor(sf::Color::Blue);
            }

            alliances[h].robots[i].drawText.setCharacterSize(static_cast<int>(sizeMulti*1.125f));
            alliances[h].robots[i].drawText.setString(std::to_string(i+1));
            alliances[h].robots[i].drawText.setFillColor(sf::Color::White);
            alliances[h].robots[i].drawText.setOrigin(static_cast<int>(sizeMulti*(-0.2)), 0);

            alliances[h].robots[i].drawLine.setFillColor(sf::Color(200,200,200,200));
            alliances[h].robots[i].drawLine.setOrigin(0,sizeMulti/4);
            i++;
        }
        h++;
    }
}

void field::updatePowUp(int t) {
    if(t > 15 && t < 149) {
        //check if powerups are done
        a = 0;
        while(a < alliances.size()) {
            b = 0;
            while(b < alliances[a].powerUpCurrLvl.size()) {
                if(alliances[a].powerUpCurrLvl[b] > 0 && alliances[a].powerUpCurrLvl[b] < 4) {
                    if(alliances[a].powerUpTime[b]+10 <= t+15) {
                        alliances[a].powerUpCurrLvl[b] = 4;
                    }
                }
                b++;
            }
            a++;
        }
        //update hasPow
        a = 0;
        hasPow = false;
        while(!hasPow && a < alliances.size()) {
            b = 0;
            while(!hasPow && b < alliances[a].powerUpCurrLvl.size()) {
                if(alliances[a].powerUpCurrLvl[b] > 0 && alliances[a].powerUpCurrLvl[b] < 4) {
                    hasPow = true;
                }
                b++;
            }
            a++;
        }
        //apply powerup
        if(!hasPow) {
            a = 0;
            while(a < alliances.size()) {
                b = 0;
                while(b < alliances[a].powerUpCurrLvl.size()) {
                    if(alliances[a].powerUpCurrLvl[b] == 0) {
                        if(!hasPow && alliances[a].powerUpTime[b] <= t+15 && elements[4+a].currentCubeNum > 0) {
                            if(b < alliances[a].powerUpCurrLvl.size()-1) {
                                if(alliances[a].powerUpLvl[b] > elements[4+a].currentCubeNum) {
                                    alliances[a].powerUpCurrLvl[b] = elements[4+a].currentCubeNum;
                                    elements[26+a].currentCubeNum += elements[4+a].currentCubeNum;
                                    elements[4+a].currentCubeNum = 0;
                                }
                                else {
                                    alliances[a].powerUpCurrLvl[b] = alliances[a].powerUpLvl[b];
                                    elements[26+a].currentCubeNum += alliances[a].powerUpLvl[b];
                                    elements[4+a].currentCubeNum -= alliances[a].powerUpLvl[b];
                                }
                                hasPow = true;
                            }
                        }
                        if(alliances[a].powerUpTime[b] <= t+15 && b == 2 && elements[4+a].currentCubeNum >= 3) {
                            elements[26+a].currentCubeNum += 3;
                            elements[4+a].currentCubeNum -= 3;
                            alliances[a].powerUpCurrLvl[b] = 4;
                        }
                    }
                    b++;
                }
                a++;
            }
        }
    }
}

void field::updateRP() {
    alliances[0].rankPoint = 0;
    alliances[1].rankPoint = 0;
    if(alliances[0].totalScore > alliances[1].totalScore) {
        alliances[0].rankPoint += 2;
    }
    else if(alliances[0].totalScore < alliances[1].totalScore) {
        alliances[1].rankPoint += 2;
    }
    else {
        alliances[0].rankPoint++;
        alliances[1].rankPoint++;
    }
    if(alliances[0].climbScore == 90) {
        alliances[0].rankPoint++;
    }
    if(alliances[1].climbScore == 90) {
        alliances[1].rankPoint++;
    }
    if(alliances[0].autoScore == 15 && alliances[0].hasSwitchAuto) {
        alliances[0].rankPoint++;
    }
    if(alliances[1].autoScore == 15 && alliances[1].hasSwitchAuto) {
        alliances[1].rankPoint++;
    }
}

void field::draw(sf::RenderWindow* rw) {
    rw->draw(backGround);
    i = 0;
    while(i < elements.size()) {
        rw->draw(elements[i].drawShape);
        if(elements[i].maxCubeNum > 0 || elements[i].initCubeNum > 0) {
            elements[i].drawText.setString(std::to_string(elements[i].currentCubeNum));
            rw->draw(elements[i].drawText);
        }
        i++;
    }
    h = 0;
    i = 0;
    while(h < alliances.size()) {
        i = 0;
        while(i < alliances[h].robots.size()) {
            angle = 1.570796325;
            if(alliances[h].robots[i].tarX-alliances[h].robots[i].posX == 0 && alliances[h].robots[i].tarY-alliances[h].robots[i].posY < 0) {
                    angle = -angle;
            }
            if(alliances[h].robots[i].tarX-alliances[h].robots[i].posX != 0) {
                angle = atan2((alliances[h].robots[i].tarY-alliances[h].robots[i].posY), (alliances[h].robots[i].tarX-alliances[h].robots[i].posX));
            }
            mag = sqrt(pow(alliances[h].robots[i].tarX-alliances[h].robots[i].posX, 2) + pow(alliances[h].robots[i].tarY-alliances[h].robots[i].posY, 2));

            alliances[h].robots[i].drawLine.setPosition(alliances[h].robots[i].posX*sizeMulti+sizeMulti*0.75,alliances[h].robots[i].posY*sizeMulti+sizeMulti*0.75);
            alliances[h].robots[i].drawLine.setSize(sf::Vector2f(mag*sizeMulti,sizeMulti/2));
            alliances[h].robots[i].drawLine.setRotation(angle*57.2957795);
            alliances[h].robots[i].drawShape.setPosition(alliances[h].robots[i].posX*sizeMulti-sizeMulti*0.15,alliances[h].robots[i].posY*sizeMulti);
            alliances[h].robots[i].drawText.setPosition(alliances[h].robots[i].posX*sizeMulti+sizeMulti*0.05,alliances[h].robots[i].posY*sizeMulti);
            if(alliances[h].robots[i].hasCube) {
                alliances[h].robots[i].drawShape.setOutlineColor(sf::Color::Yellow);
            }
            else {
                alliances[h].robots[i].drawShape.setOutlineColor(sf::Color::White);
            }

            rw->draw(alliances[h].robots[i].drawLine);
            rw->draw(alliances[h].robots[i].drawShape);
            rw->draw(alliances[h].robots[i].drawText);
            i++;
        }
        h++;
    }
    sRPW = "";
    sBPW = "";
    if(alliances[0].powerUpCurrLvl[0] > 0 && alliances[0].powerUpCurrLvl[0] < 4) {
        sRPW += "F " + std::to_string(alliances[0].powerUpCurrLvl[0]) + " ";
    }
    if(alliances[0].powerUpCurrLvl[1] > 0 && alliances[0].powerUpCurrLvl[1] < 4) {
        sRPW += "B " + std::to_string(alliances[0].powerUpCurrLvl[1]) + " ";
    }
    if(alliances[0].powerUpCurrLvl[2] == 4) {
        sRPW += "L";
    }

    if(alliances[1].powerUpCurrLvl[0] > 0 && alliances[1].powerUpCurrLvl[0] < 4) {
        sBPW += "F " + std::to_string(alliances[1].powerUpCurrLvl[0]) + " ";
    }
    if(alliances[1].powerUpCurrLvl[1] > 0 && alliances[1].powerUpCurrLvl[1] < 4) {
        sBPW += "B " + std::to_string(alliances[1].powerUpCurrLvl[1]) + " ";
    }
    if(alliances[1].powerUpCurrLvl[2] == 4) {
        sBPW += "L";
    }

    redPwrUp.setString(sRPW);
    bluePwrUp.setString(sBPW);
    redScore.setString(std::to_string(alliances[0].totalScore));
    blueScore.setString(std::to_string(alliances[1].totalScore));
    rw->draw(redPwrUp);
    rw->draw(bluePwrUp);
    rw->draw(redScore);
    rw->draw(blueScore);
    rw->draw(time);
}

std::vector<int> field::coorConv(int x, int y) {
    std::vector<int> res;
    res.push_back(53-x);
    res.push_back(26-y);
    return res;
}
