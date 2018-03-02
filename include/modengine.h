#ifndef MODENGINE_H
#define MODENGINE_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>

class modengine
{
    public:
        modengine(std::string dataPath, std::string genPath, int genSize);
        virtual ~modengine();

        std::fstream dataFile;
        std::fstream genFile;
        std::vector<std::string> inputData;
        std::vector<std::string> unsortedData;
        std::vector<int> unsortedDataScore;
        std::vector<int> unsortedDataRankPoint;
        std::vector<std::string> sortedData;
        std::vector<int> sortedDataScore;
        std::vector<int> sortedDataRankPoint;
        std::vector<std::string> outputData;

        std::string breeding(std::string p1, std::string p2);
        std::string mutate(std::string p);
        std::string mutateRobot(std::string p);
        std::string randGen(int i);
        std::string randSourceGen();

        //temp var
        int i, j, a, b, tmp;
        std::string currentLine;
        std::vector<std::string> currentLineSplited;
};

#endif // MODENGINE_H
