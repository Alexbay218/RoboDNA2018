#include "alliance.h"

alliance::alliance() {
    std::vector<robot> rtmp;
    rtmp.push_back(*(new robot()));
    rtmp.push_back(*(new robot()));
    rtmp.push_back(*(new robot()));

    robots = rtmp;
    isRed = true;

    powerUpTime.clear();
    powerUpTime.push_back(0);
    powerUpTime.push_back(0);
    powerUpTime.push_back(0);

    powerUpLvl.clear();
    powerUpLvl.push_back(1);
    powerUpLvl.push_back(1);

    init();
}

alliance::alliance(std::vector<robot> rtmp, bool iR) {
    robots = rtmp;
    isRed = iR;

    powerUpTime.clear();
    powerUpTime.push_back(0);
    powerUpTime.push_back(0);
    powerUpTime.push_back(0);

    powerUpLvl.clear();
    powerUpLvl.push_back(1);
    powerUpLvl.push_back(1);

    init();
}

alliance::alliance(std::string powUpConf, std::vector<robot> rtmp, bool iR) {
    robots = rtmp;
    isRed = iR;

    powerUpTime.clear();
    powerUpTime.push_back(std::stoi(powUpConf.substr(0, 2), NULL, 16));
    powerUpTime.push_back(std::stoi(powUpConf.substr(3, 2), NULL, 16));
    powerUpTime.push_back(std::stoi(powUpConf.substr(6, 2), NULL, 16));

    powerUpLvl.clear();
    powerUpLvl.push_back(std::stoi(powUpConf.substr(2, 1), NULL, 10));
    powerUpLvl.push_back(std::stoi(powUpConf.substr(5, 1), NULL, 10));

    init();
}

alliance::alliance(std::string allianceConf) {
    std::vector<robot> rtmp;
    rtmp.push_back(*(new robot(allianceConf.substr(8, 16), allianceConf.substr(24, 5))));
    rtmp.push_back(*(new robot(allianceConf.substr(29, 16), allianceConf.substr(45, 5))));
    rtmp.push_back(*(new robot(allianceConf.substr(50, 16), allianceConf.substr(66, 5))));
    isRed = false;
    if(allianceConf[71] == 'r') {
        isRed = true;
    }

    powerUpTime.clear();
    powerUpTime.push_back(std::stoi(allianceConf.substr(0, 2), NULL, 16));
    powerUpTime.push_back(std::stoi(allianceConf.substr(3, 2), NULL, 16));
    powerUpTime.push_back(std::stoi(allianceConf.substr(6, 2), NULL, 16));

    powerUpLvl.clear();
    powerUpLvl.push_back(std::stoi(allianceConf.substr(2, 1), NULL, 10));
    powerUpLvl.push_back(std::stoi(allianceConf.substr(5, 1), NULL, 10));

    init();
}

void alliance::init() {
    totalScore = 0;
    autoScore = 0;
    cubeScore = 0;
    vaultScore = 0;
    climbScore = 0;

    powerUpCurrLvl.clear();
    powerUpCurrLvl.push_back(0);
    powerUpCurrLvl.push_back(0);
    powerUpCurrLvl.push_back(0);
    powerUpLvl.push_back(3);

    hasSwitchAuto = false;
}

std::string getAllianceConf() {
    std::string res;
    //res.push_back()
    //TODO Finish this function
}

alliance::~alliance() {
}
