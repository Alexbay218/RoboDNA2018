#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <time.h>
#include <vector>
#include "engine.h"
#include "match.h"
#include "alliance.h"
#include "robot.h"

int main(int argc, char** argv) {
    std::string path = argv[0];
    if(path.find("\\") != std::string::npos) {
        path.resize(path.find_last_of("\\")+1);
    }
    else {
        path.resize(path.find_last_of("\/")+1);
    }
    sf::Font* font = new sf::Font();
    if(!font->loadFromFile(path + "FSEX300.ttf")) {
        std::cout << "ERROR: Cannot load font" << std::endl;
        return 0;
    }

    if(argc == 3) {
        match play((new alliance(argv[1])), (new alliance(argv[2])), 1000, static_cast<long int>(time(NULL)), 20, font, 0, 0);
        play.execMatch.join();
        return 0;
    }
    if(argc == 4) {
        match play((new alliance(argv[1])), (new alliance(argv[2])), 1000, static_cast<long int>(std::stoi(argv[3], NULL, 10)), 20, font, 0, 0);
        play.execMatch.join();
        return 0;
    }
    engine en(path, false, font);
    if(argc == 2) {
        if(static_cast<std::string>(argv[1]) == "d") {
            engine en(path, true, font);
        }
        else {
            engine en(path, false, font);
        }
    }
    /*
    std::vector<robot> rrtmp;
    rrtmp.push_back(*(new robot("aaeeeeeeeeeeeccc", "zsope")));
    rrtmp.push_back(*(new robot("aawwwwwwwwwwwwww", "szope")));
    rrtmp.push_back(*(new robot("aaaxxxxxxxxxxccc", "opsze")));

    alliance red("4523718C",rrtmp, true);
    std::cout << red.getAllianceConf() << std::endl;
    //alliance red("FF3FF3FF",rrtmp, true);

    std::vector<robot> brtmp;
    brtmp.push_back(*(new robot("aawwwwwwwwwwwwww", "zsope")));
    brtmp.push_back(*(new robot("aasssssssssssccc", "sopze")));
    brtmp.push_back(*(new robot("aawwwwwwwwwwwccc", "zsope")));

    alliance blue("FF3FF3FF", brtmp, false);
    //alliance blue("00100100aawwwwwwwwwwwwwwzsopeaassssssssssssccsopzeaaaxxxxxxxxxxcccopsze");
    std::cout << blue.getAllianceConf() << std::endl;
    //return 0;

    //match first(red, blue, 1000, static_cast<long int>(time(NULL)-9), 20, &font, 0, 0);
    //first.execMatch.join();
    //first.winThread.join();
    //*/
    /*
    std::vector<match*> matches;
    for(int j = 0;j < 9999999;j++) {
        for(int i = 0;i < 10;i++) {
            for(int j = 0;j < 10;j++) {
                matches.push_back((new match(&red, &blue, 0, static_cast<long int>(time(NULL)+i+j), 0, &font, i, j)));
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
        for(int l = 0;l < matches.size();l++) {
            delete matches[l];
        }
        matches.clear();
    }
    */
    return 0;
    //*/
}
