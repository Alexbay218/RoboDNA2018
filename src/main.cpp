#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <time.h>
#include <vector>
#include "match.h"
#include "alliance.h"
#include "robot.h"

int main(int argc, char** argv) {
    std::string path = argv[0];
    path.resize(path.find_last_of("\\")+1);
    path = path + "FSEX300.ttf";
    sf::Font font;
    if(!font.loadFromFile(path)) {
        std::cout << "ERROR: Cannot load font" << std::endl;
        //return 0;
    }

    sf::Clock clock;

    std::vector<robot> rrtmp;
    rrtmp.push_back(*(new robot("aaeeeesssssssccc", "zsope")));
    rrtmp.push_back(*(new robot("waawwweesssssccc", "zsope")));
    rrtmp.push_back(*(new robot("aaawwwwwwwwwwwww", "zsope")));

    alliance red("4533738C",rrtmp, true);
    //alliance red("FF3FF3FF",rrtmp, true);

    std::vector<robot> brtmp;
    brtmp.push_back(*(new robot("aawwwwwwwwwwwwww", "zsope")));
    brtmp.push_back(*(new robot("aasssssssssssscc", "sopze")));
    brtmp.push_back(*(new robot("aaaxxxxxxxxxxccc", "opsze")));

    alliance blue(brtmp, false);
    ///*
    match first(red, blue, 250, static_cast<long int>(time(NULL)-9), 20, &font, 0, 0);
    first.execMatch.join();
    first.winThread.join();
    //*/
    std::vector<match*> matches;
    for(int i = 0;i < 10;i++) {
        for(int j = 0;j < 10;j++) {
            matches.push_back((new match(red, blue, 0, static_cast<long int>(time(NULL)+i+j), 0, &font, i, j)));
            //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    for(int k = 0;k < matches.size();k++) {
        if(matches[k]->execMatch.joinable()) {
            matches[k]->execMatch.join();
        }
    }
    for(int l = 0;l < matches.size();l++) {
        if(matches[l]->winThread.joinable()) {
            matches[l]->winThread.join();
        }
    }

    return 0;
}
