#ifndef ALLIANCE_H
#define ALLIANCE_H

#include <string>
#include <vector>
#include "robot.h"

class alliance {
    public:
        bool isRed;
        bool hasSwitchAuto;
        std::vector<robot> robots;
        std::string powerUpOrder;

        int totalScore;
        int autoScore;
        int cubeScore;
        int vaultScore;
        int climbScore;

        //powerUp: 0 is force, 1 is boost, 2 is levitate
        std::vector<int> powerUpTime; //time requested after t = 15
        std::vector<int> powerUpLvl; //level required
        std::vector<int> powerUpCurrLvl; //0 if not occurred, 4 if done

        alliance();
        alliance(std::vector<robot> rtmp, bool iR);
        alliance(std::string powUpConf, std::vector<robot> rtmp, bool iR);
        alliance(std::string allianceConf);
        /* AllianceConf formatting
        aawwwwwwxxxxxwww | 16 char
        zsope | 5 char
        00000000 | 8 char

        00000000aawwwwwwxxxxxwwwzsopeaawwwwwwxxxxxwwwzsopeaawwwwwwxxxxxwwwzsoper  | 72 char
        | powUp | robot1        |    | robot2        |    | robot3        |    | color
        0       8              24   29              45   50              66   71

        000 | 000 | 00
        frc   bst   lev

        first two char is time after t = 15 in hex
        third char is level (since lev need three, it doesn't have any)
        */

        virtual ~alliance();
        void init();
        std::string getAllianceConf();
};

#endif //ALLIANCE_H
