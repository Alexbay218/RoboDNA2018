#ifndef MATCH_H
#define MATCH_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <time.h>
#include <random>
#include <math.h>
#include "field.h"

class match {
    public:
        field gameField;
        std::thread execMatch;
        std::thread winThread;
        sf::RenderWindow window;
        std::default_random_engine generator;
        std::uniform_int_distribution<int> climb;

        bool isRunning;
        std::string config; //switch and scale config
        //three char indicating red pos (t or b) from left switch to right
        float speedMulti; //speed multiplier

        std::string results; //red alliance config, blue alliance config, red score, blue score, red rp, blue rp

        match(alliance* red, alliance* blue, int ti, int sd, int dw, sf::Font* f, int winX, int winY);
        virtual ~match();
        void update(int t);
        void updateScore(int t);
        std::vector<int> targetCoord(int rx, int ry, bool isRed, char tar);
        std::vector<int> sourceCoord(int rx, int ry, bool isRed, std::string sourcePriority);
        bool targetDrop(int rx, int ry, bool isRed, char tar);
        bool sourceGrab(int rx, int ry, bool isRed, std::string sourcePriority);

        //temp variables
        std::vector<int> tmpTarCoord, tmpCoord;
        char target, tar;
        int a, b, c, d, i, newX, newY, cx, cy;
        bool collision;
        std::string cconfig;
};

#endif //MATCH_H
