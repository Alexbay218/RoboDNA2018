#ifndef GENENIGINE_H
#define GENENIGINE_H

#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>

class genengine {
    public:
        genengine(std::string path, int genSize);
        virtual ~genengine();

        std::string randGen(int i);
        std::string randSourceGen();

        std::fstream initFile;

        //temp var
        int tmp;
};

#endif // GENENIGINE_H
