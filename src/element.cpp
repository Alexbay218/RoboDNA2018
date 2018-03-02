#include "element.h"
#include <iostream>
element::element() {
    name = "topBluePortal";
    initCubeNum = 7;
    currentCubeNum = 7;
    maxCubeNum = 7;

    drawShape.setSize(sf::Vector2f(3*20,3*20));
    drawShape.setFillColor(sf::Color(0,0,255));
    drawShape.setPosition(0,0);
}

element::element(std::string n, int iCN, int mCN, sf::Color c, int x, int y, int w, int h, int sizeMulti) {
    name = n;
    initCubeNum = iCN;
    currentCubeNum = initCubeNum;
    maxCubeNum = mCN;

    drawShape.setSize(sf::Vector2f(w*sizeMulti,h*sizeMulti));
    drawShape.setFillColor(c);
    drawShape.setPosition(x*sizeMulti,y*sizeMulti);

    drawText.setCharacterSize(static_cast<int>(sizeMulti*1.125f));
    drawText.setString(std::to_string(currentCubeNum));
    drawText.setFillColor(sf::Color::White);
    drawText.setOrigin(drawText.getLocalBounds().width/2,drawText.getLocalBounds().height/2);
    drawText.setPosition((x+(w-1)/2)*sizeMulti+3,(y+(h-1)/2)*sizeMulti-3);
}

element::~element() {
}
