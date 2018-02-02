#ifndef ELEMENT_H
#define ELEMENT_H

#include <SFML/Graphics.hpp>

class element {
    public:
        int initCubeNum;
        int maxCubeNum;
        std::string name;

        int currentCubeNum;
        sf::RectangleShape drawShape;
        sf::Text drawText;

        element();
        element(std::string n, int iCN, int mCN, sf::Color c, int x, int y, int w, int h,int sizeMulti);
        virtual ~element();
};

#endif //ELEMENT_H
