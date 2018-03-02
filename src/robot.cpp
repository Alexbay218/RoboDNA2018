#include "robot.h"

robot::robot() {
    sourcePriority = "zsope";
    autoStrat = "aaa";
    telopNoEG = "eeeeeeeeee";
    telopEG = "eee";

    init();
}

robot::robot(std::string dna, std::string sP) {
    //enforce climb only here
    sourcePriority = "zsope";
    if(dna.size() == 16) {
        autoStrat = dna.substr(0,3);
        telopNoEG = dna.substr(3,10);
        telopEG = dna.substr(13,3);
    }
    else {
        autoStrat = "aaa";
        telopNoEG = "eeeeeeeeee";
        telopEG = "eee";
    }
    if(sP.size() == 5) {
        sourcePriority = sP;
    }
    else {
        sourcePriority = "zsope";
    }
    bool isClimbing = false;
    std::stringstream tmp;
    for(int i = 0;i < telopEG.size();i++) {
        if(telopEG[i] == 'c') {
            isClimbing = true;
        }
        if(isClimbing) {
            tmp << "c";
        }
        else {
            tmp << telopEG[i];
        }
    }
    telopEG = tmp.str();

    init();
}

void robot::init() {
    maxVel = 3;

    curMaxVel = maxVel;
    posX = 0;
    posY = 0;
    tarX = 0;
    tarY = 0;
    hasCube = true;
    hasClimb = false;
    hasAuto = false;
    hasPark = false;

    timeOut = 0;
}

robot::~robot() {
}

char robot::targetAtTime(int t) {
    if(t < 15) {
        return autoStrat[t/5];
    }
    else if(t >= 15 && t < 30) {
        return telopNoEG[0];
    }
    else if(t >= 30 && t < 120) {
        return telopNoEG[((t-30)/10)+1];
    }
    else if(t >= 120) {
        return telopEG[(t-120)/10];
    }
    else{
        return 'e';
    }
}
