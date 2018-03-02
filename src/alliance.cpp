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
    robots.clear();
    robots.push_back(robot(allianceConf.substr(8, 16), allianceConf.substr(24, 5)));
    robots.push_back(robot(allianceConf.substr(29, 16), allianceConf.substr(45, 5)));
    robots.push_back(robot(allianceConf.substr(50, 16), allianceConf.substr(66, 5)));

    powerUpTime.clear();
    powerUpTime.push_back(std::stoi(allianceConf.substr(0, 2), NULL, 16));
    powerUpTime.push_back(std::stoi(allianceConf.substr(3, 2), NULL, 16));
    powerUpTime.push_back(std::stoi(allianceConf.substr(6, 2), NULL, 16));

    powerUpLvl.clear();
    powerUpLvl.push_back(std::stoi(allianceConf.substr(2, 1), NULL, 10));
    powerUpLvl.push_back(std::stoi(allianceConf.substr(5, 1), NULL, 10));
    isRed = true;

    init();
}

void alliance::init() {
    totalScore = 0;
    autoScore = 0;
    cubeScore = 0;
    vaultScore = 0;
    climbScore = 0;
    rankPoint = 0;

    powerUpCurrLvl.clear();
    powerUpCurrLvl.push_back(0);
    powerUpCurrLvl.push_back(0);
    powerUpCurrLvl.push_back(0);
    powerUpLvl.push_back(3);

    hasSwitchAuto = false;
}

std::string alliance::getAllianceConf() {
    std::stringstream res;
    if(powerUpTime[0] < 16) {
        res << "0";
    }
    res << std::hex << powerUpTime[0];
    res << powerUpLvl[0];
    if(powerUpTime[1] < 16) {
        res << "0";
    }
    res << std::hex << powerUpTime[1];
    res << powerUpLvl[1];
    if(powerUpTime[2] < 16) {
        res << "0";
    }
    res << std::hex << powerUpTime[2];
    res << robots[0].autoStrat << robots[0].telopNoEG << robots[0].telopEG << robots[0].sourcePriority;
    res << robots[1].autoStrat << robots[1].telopNoEG << robots[1].telopEG << robots[1].sourcePriority;
    res << robots[2].autoStrat << robots[2].telopNoEG << robots[2].telopEG << robots[2].sourcePriority;
    return res.str();
}

alliance::~alliance() {
}
