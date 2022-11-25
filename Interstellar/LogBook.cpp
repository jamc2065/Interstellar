

#include "friendlyAlien.h"
#include "player.h"
#include "hiddenMap.h"
#include "LogBook.h"
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iomanip>
#include <iostream>

using namespace std;


Lbook::Lbook(){ //default constructor (reset log book)
    resetLB();
}

void Lbook::resetLB(){ //defaut values for the log book
    newPlanetName = ""; //no name until assigned

    npcAssesment = "None"; //no assement
    
    habTraits.clear(); //wipe away habbitable traits
    habTraits.push_back("None");
    
    nonHabTraits.clear(); //wipe away non-habitable traits
    nonHabTraits.push_back("None");
}


string Lbook::getP_Name(){ //getter function for planet name
    return newPlanetName;
}

string Lbook::getP_Diameter(){ //getter function for planet diameter
    return sizeDiameter;
}

string Lbook::getCarry_Cap(){ //getter function for carrying capacity
    return carryCap;
}

string Lbook::getH_traits(){ //getter function for habitable traits on planet
    string H_traits = ""; //store blank until found trait
    for (int i = 0; i < habTraits.size(); i++){ //for loop storing the habitable traits
    
        H_traits += habTraits[i];
        H_traits += " ";
    
    }
    return H_traits; //return traits that were found
}

string Lbook::getNH_traits(){ //getter function for non-habitable traits on planet
    string NH_traits = ""; //store blank until found trait
    for (int i = 0; i < nonHabTraits.size(); i++){ //for loop storing the non -habitable traits
        NH_traits += nonHabTraits[i];
        NH_traits += " ";
    
    }
    return NH_traits; //return traits that were found
}

string Lbook::getNPC_a(){ //getter function for npc's assesment
    return npcAssesment;
}


void Lbook::setP_Name(){ //setter for the planets name
    string strOne = ""; //empty 
    string strTwo = ""; //empty

    string ascciArr[26] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"}; //array of all the letters
    string numArr[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"}; //array of numbers 1-9

    for (int i = 0; i < 6; i++){ //for loop to store 6 characters
        int rInt = rand() % 2 + 1; //1(number) or 2(letter)
        if (rInt == 1){ //if 1 is the random number
            int numR = rand() % 8; //random 1-9
            strOne += numArr[numR]; //add random number to the number array and to strOne
        }
        if (rInt == 2){ //if 2 is the random number
            int ascciR = rand() % 25; //random letter in alphebet 
            strOne += ascciArr[ascciR]; //add random lettter to the assci array and to strOne
        }
    }

    int countline = 1;
    int pickline = rand() % 3125 + 1; //random number 1-3125
    string line; //string for each line
    ifstream input; //create ifstream
    input.open("four_letter_words.txt"); //open file
    while(getline(input, line)){ //go through each line in file
        if(pickline == countline){ 
            strTwo = line;
        }
        countline++; //add the number of lines read
    }

    newPlanetName += strOne; //random 6 character 
    newPlanetName += "-"; //seperator
    newPlanetName += strTwo; //random four letter word from file
}

void Lbook::setP_Diameter(){ //setter for planets diameter
    sizeDiameter = ""; //empty
    int x = rand() %  16 + 2; //random between 2 and 16
    double nSize = x * 0.5; //half of x
    sizeDiameter = to_string(nSize); //convert string diameter to double nSize
}

void Lbook::setCarry_Cap(){ //setter for planets carrying capacity
    carryCap = ""; //empty
    double cc = stod(sizeDiameter);//convert int size diameter to double size diameter
    cc *= 615.0; //given formula
    carryCap = to_string(cc); //convert carrying capacity to string
}

void Lbook::setH_traits(string addHT){ //seter for habitale traits

    if (habTraits[0] == "None"){ //if no traits have been found yet
        habTraits.clear(); //get rid of the word clear in the vector
        habTraits.push_back(addHT); //replace with the habitable trait
    }
    else{
        habTraits.push_back(addHT); //add the new habitable trait to the vector
    }
}

void Lbook::setNH_traits(string addNHT){ //seter for non-habitale traits

    if (nonHabTraits[0] == "None"){ //if no traits have been found yet
        nonHabTraits.pop_back(); //get rid of the word clear in the vector
        nonHabTraits.push_back(addNHT); //replace with the non-habitable trait
    }
    else{
        nonHabTraits.push_back(addNHT); //add the new non-habitable trait to the vector
    }
}

void Lbook::setNPC_a(bool alienA){ //setter for aliens assesment
    if (alienA == true){ //if found true, set assesment to habitable
        npcAssesment = "Habitable";
    }
    else{ //if found false, set assesment to non-habitable
        npcAssesment = "Non-habitable";
    }
}

// string newPlanetName;
// string sizeDiameter;
// string carryCap;
// vector<string> habTraits;
// vector<string> nonHabTraits;
// string npcAssesment;