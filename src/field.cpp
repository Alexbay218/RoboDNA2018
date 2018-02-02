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

field::field(alliance red, alliance blue, int sm) {
    std::vector<alliance> atmp;
    atmp.push_back(red);
    atmp.push_back(blue);

    alliances = atmp;
    std::cout << alliances.size() << std::endl;
    sizeMulti = sm;
    init();
}

field::~field() {
}

void field::init() {
    backGround.setSize(sf::Vector2f(54*sizeMulti,27*sizeMulti));
    backGround.setFillColor(sf::Color(150,150,150));
    std::vector<element> etmp;
    etmp.push_back(*(new element("topBluePortal",7,0, sf::Color::Blue,0,0,3,3,sizeMulti)));         //00
    etmp.push_back(*(new element("botBluePortal",7,0, sf::Color::Blue,0,24,3,3,sizeMulti)));        //01
    etmp.push_back(*(new element("topRedPortal",7,0, sf::Color::Red,51,0,3,3,sizeMulti)));          //02
    etmp.push_back(*(new element("botRedPortal",7,0, sf::Color::Red,51,24,3,3,sizeMulti)));         //03
    etmp.push_back(*(new element("redExchange",0,9, sf::Color::Red,0,9,3,4,sizeMulti)));            //04
    etmp.push_back(*(new element("blueExchange",0,9, sf::Color::Blue,51,14,3,4,sizeMulti)));        //05
    etmp.push_back(*(new element("redAutoLine",0,0, sf::Color(175,175,175),10,0,1,27,sizeMulti)));  //06
    etmp.push_back(*(new element("blueAutoLine",0,0, sf::Color(175,175,175),43,0,1,27,sizeMulti))); //07
    etmp.push_back(*(new element("redPowCubeZone",10,0, sf::Color(200,0,0),9,12,3,3,sizeMulti)));       //08
    etmp.push_back(*(new element("bluePowCubeZone",10,0, sf::Color(0,0,200),42,12,3,3,sizeMulti)));    //09
    etmp.push_back(*(new element("redPlatformZone",0,0, sf::Color(200,0,0),22,8,4,11,sizeMulti)));      //10
    etmp.push_back(*(new element("bluePlatformZone",0,0, sf::Color(0,0,200),28,8,4,11,sizeMulti)));    //11
    etmp.push_back(*(new element("redBackZone",6,0, sf::Color(200,0,0),16,9,2,9,sizeMulti)));           //12
    etmp.push_back(*(new element("blueBackZone",6,0, sf::Color(0,0,200),36,9,2,9,sizeMulti)));         //13
    etmp.push_back(*(new element("redSwitchBlock",0,0, sf::Color(75,75,75),12,11,4,5,sizeMulti)));  //14
    etmp.push_back(*(new element("blueSwitchBlock",0,0, sf::Color(75,75,75),38,11,4,5,sizeMulti))); //15
    etmp.push_back(*(new element("scaleBlock",0,0, sf::Color(75,75,75),26,9,2,9,sizeMulti)));       //16
    etmp.push_back(*(new element("redTopSwitch",0,60, sf::Color::Yellow,12,8,4,3,sizeMulti)));      //17
    etmp.push_back(*(new element("redBotSwitch",0,60, sf::Color::Yellow,12,16,4,3,sizeMulti)));     //18
    etmp.push_back(*(new element("blueTopSwitch",0,60, sf::Color::Yellow,38,8,4,3,sizeMulti)));     //19
    etmp.push_back(*(new element("blueBotSwitch",0,60, sf::Color::Yellow,38,16,4,3,sizeMulti)));    //20
    etmp.push_back(*(new element("topScale",0,60, sf::Color::Yellow,25,6,4,3,sizeMulti)));          //21
    etmp.push_back(*(new element("botScale",0,60, sf::Color::Yellow,25,18,4,3,sizeMulti)));         //22
    etmp.push_back(*(new element("redScoreBG",0,0, sf::Color(235,0,0),0,27,27,7,sizeMulti)));
    etmp.push_back(*(new element("blueScoreBG",0,0, sf::Color(0,0,235),27,27,27,7,sizeMulti)));
    etmp.push_back(*(new element("sidePanelBG",0,0, sf::Color(200,200,200),54,0,7,34,sizeMulti)));
    elements = etmp;

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

void field::draw(sf::RenderWindow* rw) {
    rw->draw(backGround);
    int i = 0;
    while(i < elements.size()) {
        rw->draw(elements[i].drawShape);
        if(elements[i].maxCubeNum > 0 || elements[i].initCubeNum > 0) {
            elements[i].drawText.setString(std::to_string(elements[i].currentCubeNum));
            rw->draw(elements[i].drawText);
        }
        i++;
    }
    int h = 0;
    i = 0;
    while(h < alliances.size()) {
        i = 0;
        while(i < alliances[h].robots.size()) {
            double angle = 1.570796325;
            if(alliances[h].robots[i].tarX-alliances[h].robots[i].posX != 0) {
                angle = atan2((alliances[h].robots[i].tarY-alliances[h].robots[i].posY), (alliances[h].robots[i].tarX-alliances[h].robots[i].posX));
            }
            double mag = sqrt(pow(alliances[h].robots[i].tarX-alliances[h].robots[i].posX, 2) + pow(alliances[h].robots[i].tarY-alliances[h].robots[i].posY, 2));
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
}

std::vector<int> field::coorConv(int x, int y) {
    std::vector<int> res;
    res.push_back(53-x);
    res.push_back(26-y);
    return res;
}
