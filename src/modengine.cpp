#include "modengine.h"

modengine::modengine(std::string dataPath, std::string genPath, int genSize) {
    srand(time(NULL));

    inputData.clear();
    unsortedData.clear();
    unsortedDataScore.clear();
    unsortedDataRankPoint.clear();
    sortedData.clear();
    sortedDataScore.clear();
    sortedDataRankPoint.clear();
    outputData.clear();

    dataFile.open(dataPath, std::ios::in);
    while(getline(dataFile, currentLine)) {
        inputData.push_back(currentLine);
    }
    inputData.erase(inputData.begin());
    dataFile.close();

    i = 0;
    while(i < inputData.size()) {
        currentLineSplited.clear();
        std::string s = inputData[i];
        std::string delimiter = ",";

        int pos = 0;
        std::string token;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0, pos);
            currentLineSplited.push_back(token);
            s.erase(0, pos + delimiter.length());
        }
        currentLineSplited.push_back(s);

        unsortedData.push_back(currentLineSplited[0]);
        unsortedDataScore.push_back(std::stoi(currentLineSplited[3], NULL, 10));
        unsortedDataRankPoint.push_back(std::stoi(currentLineSplited[5], NULL, 10));
        unsortedData.push_back(currentLineSplited[1]);
        unsortedDataScore.push_back(std::stoi(currentLineSplited[4], NULL, 10));
        unsortedDataRankPoint.push_back(std::stoi(currentLineSplited[6], NULL, 10));
        i++;
    }

    i = 0;
    while(i < unsortedData.size()) {
        j = 0;
        a = 0;
        while(j < sortedData.size()) {
            if(unsortedDataRankPoint[i] <= sortedDataRankPoint[j]) {
                a = j;
                j = sortedData.size();
            }
            j++;
        }
        sortedData.insert(sortedData.begin()+a, unsortedData[i]);
        sortedDataScore.insert(sortedDataScore.begin()+a, unsortedDataScore[i]);
        sortedDataRankPoint.insert(sortedDataRankPoint.begin()+a, unsortedDataRankPoint[i]);
        i++;
    }
    unsortedData = sortedData;
    sortedData.clear();
    unsortedDataScore = sortedDataScore;
    sortedDataScore.clear();
    unsortedDataRankPoint = sortedDataRankPoint;
    sortedDataRankPoint.clear();

    i = 0;
    while(i < unsortedData.size()) {
        j = 0;
        a = 0;
        while(j < sortedData.size()) {
            if(unsortedDataScore[i] >= sortedDataScore[j]) {
                a = j;
                j = sortedData.size();
            }
            j++;
        }
        sortedData.insert(sortedData.begin()+a, unsortedData[i]);
        sortedDataScore.insert(sortedDataScore.begin()+a, unsortedDataScore[i]);
        sortedDataRankPoint.insert(sortedDataRankPoint.begin()+a, unsortedDataRankPoint[i]);
        i++;
    }
    i = 0;
    while(i < sortedData.size()/8) {
        outputData.push_back(breeding(sortedData[i],sortedData[sortedData.size()/4-i]));
        i++;
    }

    i = 0;
    while(i < sortedData.size()/3) {
        outputData.push_back(mutate(sortedData[i]));
        i++;
    }

    while(outputData.size() < genSize) {
        outputData.push_back(randGen(rand()%2));
    }
    genFile.open(genPath, std::ios::out | std::ios::trunc);
    i = 0;
    while(i < outputData.size()) {
        genFile << outputData[i] << "\n";
        i++;
    }
    genFile.close();
}

std::string modengine::breeding(std::string p1, std::string p2) {
    std::stringstream res;
    if(rand() % 2 == 0) {
        res << p1.substr(0, 3);
    }
    else {
        res << p2.substr(0, 3);
    }
    if(rand() % 2 == 0) {
        res << p1.substr(3, 3);
    }
    else {
        res << p2.substr(3, 3);
    }
    if(rand() % 2 == 0) {
        res << p1.substr(6, 2);
    }
    else {
        res << p2.substr(6, 2);
    }
    int c = 0;
    while(c < p1.size() - 8) {
        if(rand() % 2 == 0) {
            res << p1[c+8];
        }
        else {
            res << p2[c+8];
        }
        c++;
    }
    return res.str();
}

std::string modengine::mutate(std:: string p) {
    std::stringstream res;
    int f = std::stoi(p.substr(0,2), NULL, 16) - 20 + (rand() % 40);
    if(f < 0) {
        f = 0;
    }
    if(f > 134) {
        f = 134;
    }
    if(f < 16) {
        res << "0";
    }
    res << std::hex << f;
    int fl = std::stoi(p.substr(2,1), NULL, 10);
    if(rand() % 2 == 0) {
        fl = rand() % 3 + 1;
    }
    res << fl;

    int b = std::stoi(p.substr(3,2), NULL, 16) - 20 + (rand() % 40);
    if(b < 0) {
        b = 0;
    }
    if(b > 134) {
        b = 134;
    }
    if(b < 16) {
        res << "0";
    }
    res << std::hex << b;
    int bl = std::stoi(p.substr(5,1), NULL, 10);
    if(rand() % 2 == 0) {
        bl = rand() % 3 + 1;
    }
    res << bl;

    int l = std::stoi(p.substr(6,2), NULL, 16) - 20 + (rand() % 40);
    if(l < 0) {
        l = 0;
    }
    if(l > 134) {
        l = 134;
    }
    if(l < 16) {
        res << "0";
    }
    res << std::hex << l;
    res << mutateRobot(p.substr(8, 21)) << mutateRobot(p.substr(29, 21)) << mutateRobot(p.substr(50, 21));
    return res.str();
}

std::string modengine::mutateRobot(std::string p) {
    std::stringstream res;
    int c = 0;
    while(c < p.size()-5) {
        if(rand() % 2 == 0) {
            res << p[c];
        }
        else if(c < 3) {
            int tmp = rand() % 4;
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
        else if(c >= 3 && c < 13){
            int tmp = rand() % 4;
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
        else if(c >= 13) {
            int tmp = rand() % 5;
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
        c++;
    }
    if(rand()%2 == 0) {
        res << p.substr(16,5);
    }
    else {
        res << randSourceGen();
    }
    return res.str();
}

std::string modengine::randGen(int i) {
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

std::string modengine::randSourceGen() {
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

modengine::~modengine() {
}
