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
        sf::Text time;

        field();
        field(alliance red, alliance blue, int sm);
        virtual ~field();
        void init();
        void draw(sf::RenderWindow* rw);
        void updatePowUp(int t);
        std::vector<int> coorConv(int x, int y);

        //temp variable
        int a, b;
        bool hasPow; //checked if a pow is going
};

#endif //FIELD_H
