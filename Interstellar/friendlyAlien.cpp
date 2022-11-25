

#include "friendlyAlien.h"
#include "player.h"
#include "hiddenMap.h"
#include "LogBook.h"
#include <fstream>
#include <cstring>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <iostream>

using namespace std;


FAlien::FAlien(){ //default constuctor
    hasComms = false; //default settinng that user starts with no comms
}

bool FAlien::getComms(){ //getter funcion for comms
    return hasComms;
}

void FAlien::setComms(bool co){ //setter function to set comms
    hasComms = co;
}

bool FAlien::getIsPlanHab(){ //getter function for testing the habitablity
    return isPlanHab;
}