#ifndef MATCH_H
#define MATCH_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <time.h>
#include "field.h"

class match {
    public:
        field gameField;
        sf::Clock clock;
        std::thread execMatch;
        std::thread winThread;
        sf::RenderWindow window;

        bool isRunning;
        std::string config; //switch and scale config
        //three char indicating red pos (t or b) from left switch to right
        float speedMulti; //speed multiplier

        match(alliance red, alliance blue,int ti, int sd, int dw, sf::Font* f, int winX, int winY);
        virtual ~match();
        void update(int t);
        std::vector<int> targetCoord(int rx, int ry, bool isRed, char tar);
        std::vector<int> sourceCoord(int rx, int ry, bool isRed, std::string sourcePriority);
        bool targetDrop(int rx, int ry, bool isRed, char tar);
        bool sourceGrab(int rx, int ry, bool isRed, std::string sourcePriority);
};

#endif //MATCH_H
