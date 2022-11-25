

/*
* define functions in hostile alien.h
**/

#include "hostileAlien.h"
#include "player.h"
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;


HAlien::HAlien(){  //default constructor
    HAlienName = ""; //alien has no name until asigned
    randLVLhAlien = 0; //alien level is zero until assigned
    randLVLplayer = 0; //player level is zero until assigned
}


//getters
string HAlien::getHAlienName(){ //getter function for the hostile aliens name
    return HAlienName;
}

int HAlien::getRandLVL_A(){ //getter function for the hostile aliens level (random)
    return randLVLhAlien;
}

int HAlien::getRandLVL_U(){ //getter function for the users level (random)
    return randLVLplayer;
}


//setters
void HAlien::setHAlienName(){ //setter for hostile aliens name
    vector<string> aNames; //create vector to store all of the potential alien names
    string line; //string to store each name in the file
    ifstream input; //create ifstream
    input.open("alien_names.txt"); //open the aliens namee file  
    while(getline(input, line)){ //add every name in the file to the vector
        aNames.push_back(line);     
    }
    int pick = rand() % 19; //pick a random number 1-19
    HAlienName = aNames[pick];  // assign the alien name to the name on line number 'pick'
}

void HAlien::setRandLVLalien(){ //setter function for allien level (random)
    randLVLhAlien = rand() % 6 + 1; //level will be 1-6
}

void HAlien::setRandLVLplayer(){ //setter function for users level (random)
    randLVLplayer = rand() % 6 + 1; //level will by 1-6
}