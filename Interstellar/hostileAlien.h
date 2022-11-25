

/*
* class defining functions that create enemy alien
**/

#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <iostream>
#include "player.h"
using namespace std;

#ifndef HALIEN_H    // define class in one place only
#define HALIEN_H

class HAlien
{
    private:
        string HAlienName;      // holds name from alien name list
        int randLVLhAlien;      // give alien rand lvl for fighting
        int randLVLplayer;      // same but for player

    public:
        HAlien();

        string getHAlienName(); //getter for the hostile aliens names
        int getRandLVL_A(); //getter for the hostile aliens level
        int getRandLVL_U(); //getter for the aliens level

        void setHAlienName(); //setter for hostile aliens name
        void setRandLVLalien(); //setter for the hostile aliens level
        void setRandLVLplayer(); //getter for the aliens level
};

#endif