#ifndef ALLIANCE_H
#define ALLIANCE_H

#include <vector>
#include "robot.h"

class alliance {
    public:
        bool isRed;
        std::vector<robot> robots;
        std::vector<int> powerUpTimeStamp;
        std::string powerUpOrder;

        alliance();
        alliance(std::vector<robot> rtmp, bool iR);
        virtual ~alliance();
};

#endif //ALLIANCE_H
