#ifndef ENGINE_H
#define ENGINE_H

#include <SFML/Graphics.hpp>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include "genengine.h"
#include "modengine.h"
#include "match.h"
#include "alliance.h"

class engine {
    public:
        engine(std::string rootPath, bool demo, sf::Font* font);
        virtual ~engine();

        int currentGeneration;
        std::vector<std::string> currentAllianceGen;
        std::fstream currentGenFile;
        std::fstream currentDataFile;

        //temp vars
        std::string currentLine;
        std::vector<int> testedAlliances;
        std::vector<match*> matches;
        bool isInTA;
        int i, c;
};

#endif // ENGINE_H
