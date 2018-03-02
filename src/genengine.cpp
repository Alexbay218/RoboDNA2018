#include "genengine.h"

genengine::genengine(std::string path, int genSize) {
    srand(time(NULL));
    initFile.open(path + "gen0.csv", std::ios::out | std::ios::trunc);
    std::cout << "Generate New Alliances" << std::endl;
    for(int i = 0;i < genSize;i++) {
        initFile << randGen(rand() % 2) << "\n";
    }
    initFile.close();
}

std::string genengine::randGen(int i) {
    std::stringstream res;
    int c = rand() % 134 + 1;
    if(c < 16) {
        res << "0";
    }
    res << std::hex << c << rand() % 3 + 1;

    c = rand() % 134 + 1;
    if(c < 16) {
        res << "0";
    }
    res << std::hex << c << rand() % 3 + 1;

    c = rand() % 134 + 1;
    if(c < 16) {
        res << "0";
    }
    res << std::hex << c;
    for(int c = 0;c < 3;c++) {
        if(i <= 0) {
            tmp = rand() % 4;
            if(tmp == 0) {
                res << "e";
            }
            else if(tmp == 1) {
                res << "a";
            }
            else if(tmp == 2) {
                res << "w";
            }
            else {
                res << "s";
            }
            for(int a = 0;a < 2;a++) {
                if(rand() % 3 == 0) {
                    tmp = rand() % 4;
                    if(tmp == 0) {
                        res << "e";
                    }
                    else if(tmp == 1) {
                        res << "a";
                    }
                    else if(tmp == 2) {
                        res << "w";
                    }
                    else {
                        res << "s";
                    }
                }
                else {
                    res << res.str().back();
                }
            }
            tmp = rand() % 4;
            if(tmp == 0) {
                res << "e";
            }
            else if(tmp == 1) {
                res << "w";
            }
            else if(tmp == 2) {
                res << "x";
            }
            else {
                res << "s";
            }
            for(int a = 0;a < 9;a++) {
                if(rand() % 5 <= 1) {
                    tmp = rand() % 4;
                    if(tmp == 0) {
                        res << "e";
                    }
                    else if(tmp == 1) {
                        res << "w";
                    }
                    else if(tmp == 2) {
                        res << "x";
                    }
                    else {
                        res << "s";
                    }
                }
                else {
                    res << res.str().back();
                }
            }
            tmp = rand() % 5;
            if(tmp == 0) {
                res << "e";
            }
            else if(tmp == 1) {
                res << "w";
            }
            else if(tmp == 2) {
                res << "x";
            }
            else if(tmp == 3) {
                res << "s";
            }
            else {
                res << "c";
            }
            for(int a = 0;a < 2;a++) {
                if(rand() % 3 == 0) {
                    tmp = rand() % 5;
                    if(tmp == 0) {
                        res << "e";
                    }
                    else if(tmp == 1) {
                        res << "w";
                    }
                    else if(tmp == 2) {
                        res << "x";
                    }
                    else if(tmp == 3) {
                        res << "s";
                    }
                    else {
                        res << "c";
                    }
                }
                else {
                    res << res.str().back();
                }
            }
        }
        else {
            for(int a = 0;a < 3;a++) {
                tmp = rand() % 4;
                if(tmp == 0) {
                    res << "e";
                }
                else if(tmp == 1) {
                    res << "a";
                }
                else if(tmp == 2) {
                    res << "w";
                }
                else {
                    res << "s";
                }
            }
            for(int a = 0;a < 10;a++) {
                tmp = rand() % 4;
                if(tmp == 0) {
                    res << "e";
                }
                else if(tmp == 1) {
                    res << "w";
                }
                else if(tmp == 2) {
                    res << "x";
                }
                else {
                    res << "s";
                }
            }
            for(int a = 0;a < 3;a++) {
                tmp = rand() % 5;
                if(tmp == 0) {
                    res << "e";
                }
                else if(tmp == 1) {
                    res << "w";
                }
                else if(tmp == 2) {
                    res << "x";
                }
                else if(tmp == 3) {
                    res << "s";
                }
                else {
                    res << "c";
                }
            }
        }
        res << randSourceGen();
    }
    return res.str();
}

std::string genengine::randSourceGen() {
    std::vector<char> unpicked;
    unpicked.push_back('e');
    unpicked.push_back('z');
    unpicked.push_back('s');
    unpicked.push_back('o');
    unpicked.push_back('p');

    std::vector<char> results;
    while(results.size() < 5) {
        tmp = rand() % unpicked.size();
        results.push_back(unpicked[tmp]);
        unpicked.erase(unpicked.begin()+tmp);
    }
    std::stringstream res;
    for(int c = 0;c < results.size();c++) {
        res << results[c];
    }
    return res.str();
}

genengine::~genengine() {
    //dtor
}
