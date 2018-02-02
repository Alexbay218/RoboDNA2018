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
        return 0;
    }

    sf::Clock clock;

    std::vector<robot> rrtmp;
    rrtmp.push_back(*(new robot("aawwwwwwxxxxxwww", "zsope")));
    rrtmp.push_back(*(new robot("aaassssssssssscc", "sopze")));
    rrtmp.push_back(*(new robot("aaaeeeeewwwwwccc", "zsope")));

    alliance red(rrtmp, true);

    std::vector<robot> brtmp;
    brtmp.push_back(*(new robot("aawwwwwwwwwwwwww", "zsope")));
    brtmp.push_back(*(new robot("aasssssssssssscc", "sopze")));
    brtmp.push_back(*(new robot("aaaxxxxxxxxxxccc", "opsze")));

    alliance blue(brtmp, false);

    match m1(red, blue, 250,  static_cast<long int>(time(NULL)), 20, &font, 0, 0);
    m1.winThread.join();
    m1.execMatch.join();
    /*
    match m2(red, blue, 100,  static_cast<long int>(time(NULL)), 7, &font, 0, 1);
    match m3(red, blue, 100,  static_cast<long int>(time(NULL)), 7, &font, 0, 2);
    m1.execMatch.join();
    m2.execMatch.join();
    m3.execMatch.join();
    //*/
    return 0;
}
