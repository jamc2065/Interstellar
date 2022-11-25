

#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <iostream>
using namespace std;

#ifndef LBOOK_H
#define LBOOK_H

class Lbook
{
    private:
        string newPlanetName; //planet name
        string sizeDiameter; //planet diameter
        string carryCap; //planet carrying capacity
        vector<string> habTraits; //vector storing all habbitable traits
        vector<string> nonHabTraits; //vector storing all non-habbitable traits
        string npcAssesment; //npc decision if planet is habitable or not

    public:
        Lbook(); //default constructor

        string getP_Name(); //getter for planets name
        string getP_Diameter(); //getter for planets diameter
        string getCarry_Cap(); //getter for plantes carring capacity
        string getH_traits(); //get habbitable traits
        string getNH_traits(); //get non-habitable traits
        string getNPC_a(); //see if planet is habbitabe or not

        void setP_Name(); //setter for planets name
        void setP_Diameter(); //setter for planets diameter
        void setCarry_Cap(); //setter for plantes carring capacity
        void setH_traits(string addHT); //set habbitable traits
        void setNH_traits(string addHT); //set non-habitable traits
        void setNPC_a(bool alienA); //see if planet is habbitabe or not

        void resetLB();
};

#endif

// Jake’s Log Book page #2: Planet IU9G5G-Tore
// Planet size (in diameter of 1000 kms): 2
// Planet’s carrying capacity (in millions): 1230
// Habitable traits discovered: Oxygen, Water
// Non-habitable traits discovered: None
// Friendly alien assessment: None