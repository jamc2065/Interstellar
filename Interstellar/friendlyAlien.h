

#include <fstream>
#include <string>
#include <iomanip>
#include <iostream>
#include "LogBook.h"
#include "hiddenMap.h"
#include "player.h"
using namespace std;

#ifndef FALIEN_H
#define FALIEN_H

class FAlien
{   
    private:
        bool isPlanHab; //true or false if a plannet is habitable 
        bool hasComms; //true or false if the user has comms

    public:
        FAlien();                 // constructor

        bool getIsPlanHab(); //getter testing true or false if a plannet is habitable 
        bool getComms(); //getter testing true or false if a user has comms
        void setComms(bool co); //setter for comms
};

#endif
