#include "engine.h"

engine::engine(std::string rootPath, bool demo, sf::Font* font) {
    std::srand(time(NULL));

    currentGeneration = 0;
    if(!std::experimental::filesystem::exists(rootPath + "data")) {
        std::experimental::filesystem::create_directory(rootPath + "data\\");
    }
    while(std::experimental::filesystem::exists(rootPath + "data\\gen" + std::to_string(currentGeneration) + ".csv")) {
        currentGeneration++;
    }

    if(currentGeneration <= 0) {
        genengine gen(rootPath + "data\\", 2500);
    }
    else {
        if(std::experimental::filesystem::exists(rootPath + "data\\gen" + std::to_string(currentGeneration) + ".csv")) {
            std::experimental::filesystem::remove(rootPath + "data\\gen" + std::to_string(currentGeneration) + ".csv");
        }
        if(std::experimental::filesystem::exists(rootPath + "data\\data" + std::to_string(currentGeneration) + ".csv")) {
            std::experimental::filesystem::remove(rootPath + "data\\data" + std::to_string(currentGeneration) + ".csv");
        }
        currentGeneration--;
    }
    currentGenFile.open(rootPath + "data\\gen" + std::to_string(currentGeneration) + ".csv", std::ios::in);
    std::cout << "Opening: " << rootPath + "data\\gen" + std::to_string(currentGeneration) + ".csv" << std::endl;
    currentAllianceGen.clear();
    testedAlliances.clear();

    while(getline(currentGenFile, currentLine)) {
        currentAllianceGen.push_back(currentLine);
    }
    currentGenFile.close();
    std::cout << "Closing: " << rootPath + "data\\gen" + std::to_string(currentGeneration) + ".csv" << std::endl;

    currentDataFile.open(rootPath + "data\\data" + std::to_string(currentGeneration) + ".csv", std::ios::out | std::ios::trunc);
    std::cout << "Writing: " << rootPath + "data\\data" + std::to_string(currentGeneration) + ".csv" << std::endl;
    currentDataFile << "Red Alliance Config, Blue Alliance Config, Seed, Red Score, Blue Score, Red Ranking Points, Blue Ranking Points\n";
    while(testedAlliances.size() < currentAllianceGen.size()) {
        c = rand() % currentAllianceGen.size();
        isInTA = false;
        i = 0;
        while(i < testedAlliances.size()) {
            if(testedAlliances[i] == c) {
                isInTA = true;
                i = testedAlliances.size();
            }
            i++;
        }
        if(!isInTA) {
            testedAlliances.push_back(c);
        }
    }
    if(testedAlliances.size() % 2 != 0) {
        testedAlliances.erase(testedAlliances.end());
    }
    std::cout << "Starting Matches" << std::endl;
    if(demo) {
        std::cout << "Demo Mode" << std::endl;
        i = 0;
        while(i <= testedAlliances.size()/2) {
            for(int x = 0;x < 2 && i < testedAlliances.size()/2;x++) {
                for(int y = 0;y < 2 && i < testedAlliances.size()/2;y++) {
                    if(i < testedAlliances.size()/2) {
                        matches.push_back((new match((new alliance(currentAllianceGen[i])), (new alliance(currentAllianceGen[testedAlliances.size()-i-1])), 500, static_cast<long int>(time(NULL)+x+y), 12, font, x, y)));
                        std::this_thread::sleep_for(std::chrono::milliseconds(250));
                        i++;
                    }
                }
            }
            for(int s = 0;s < matches.size();s++) {
                matches[s]->execMatch.join();
                if(matches[s]->winThread.joinable()) {
                    matches[s]->winThread.join();
                }
                currentDataFile << matches[s]->results << "\n";
            }
            for(int j = 0;j < matches.size();j++) {
                delete matches[j];
            }
            matches.clear();
        }
    }
    else {
        std::cout << "Regular Mode" << std::endl;
        i = 0;
        while(i < testedAlliances.size()/2) {
            for(int c = 0;c < 8;c++) {
                if(i < testedAlliances.size()/2) {
                    matches.push_back((new match((new alliance(currentAllianceGen[i])), (new alliance(currentAllianceGen[testedAlliances.size()-i-1])), 0, static_cast<long int>(time(NULL)+i), 0, font, 0, 0)));
                    i++;
                }
            }
            for(int s = 0;s < matches.size();s++) {
                matches[s]->execMatch.join();
                if(matches[s]->winThread.joinable()) {
                    matches[s]->winThread.join();
                }
                currentDataFile << matches[s]->results << "\n";
            }
            for(int j = 0;j < matches.size();j++) {
                delete matches[j];
            }
            matches.clear();
        }
    }
    currentDataFile.close();
    std::cout << "Closing: " << rootPath + "data\\data" + std::to_string(currentGeneration) + ".csv" << std::endl;
    modengine mod(rootPath + "data\\data" + std::to_string(currentGeneration) + ".csv", rootPath + "data\\gen" + std::to_string(currentGeneration+1) + ".csv", 2500);
}

engine::~engine() {
}
