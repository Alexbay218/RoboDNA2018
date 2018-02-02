#include "alliance.h"

alliance::alliance() {
    std::vector<robot> rtmp;
    rtmp.push_back(*(new robot()));
    rtmp.push_back(*(new robot()));
    rtmp.push_back(*(new robot()));

    isRed = true;
    robots = rtmp;
}

alliance::alliance(std::vector<robot> rtmp, bool iR) {
    robots = rtmp;
    isRed = iR;
}

alliance::~alliance() {
}
