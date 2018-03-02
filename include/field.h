#ifndef FIELD_H
#define FIELD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "alliance.h"
#include "element.h"

class field {
    public:
        std::vector<alliance> alliances;
        std::vector<element> elements;
        sf::RectangleShape backGround;
        int sizeMulti;
        sf::Text redScore;
        sf::Text blueScore;
        sf::Text redPwrUp;
        sf::Text bluePwrUp;
        sf::Text time;

        field();
        field(alliance* red, alliance* blue, int sm, sf::Font* f);
        virtual ~field();
        void init();
        void draw(sf::RenderWindow* rw);
        void updatePowUp(int t);
        void updateRP();
        std::vector<int> coorConv(int x, int y);

        //temp variable
        int a, b, i, h;
        double angle, mag;
        bool hasPow; //checked if a pow is going
        std::string sRPW, sBPW;
};

#endif //FIELD_H
