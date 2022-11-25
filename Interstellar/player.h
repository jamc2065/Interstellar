

#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <iostream>
using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

class Player
{
    private:
        string playerName;
        string crewMates[2];
        
        int fuelInGal;
        int amountMoney;
        int spaceSuitLVL;

        int numMedKits;
        int numLightSaber;
        int numBlaster;
        int numBeamGun;
        bool haveTranDevice;

        double playerHealth;
        double suitHealth;

        int numPlanetsExplored;
        int numHabPlanets_Accurate;
        int numHabPlanets_Inaccurate;

        int humansSaved;
        int humansPerished;

        bool waltTF;

    public:
        //Getters*********
        Player();                 //default constructor

        void resetSH();             //reset suit health

        string getPlayerName();     //returns player name
        
        int getAmountFuel();       //returns fuel in gallons        
        int getAmountMoney();      //returns amount of money
        int getSpaceSuitLevel();   //returns space suit level

        int getNumMedKit();     //returns number of med kits
  
        int getNumLSaber();     //returns number of light sabers
        int getNumBlaster();    //returns number of blasters
        int getNumBeamGun();    //returns number of beam guns

        bool checkTranDevice();  //returns t/f for translator device 

        string getSelectCrewNames(int index);  //returns crewmate name at gievn index

        double getPlayerH();  //returns player health
        double getSuitH();    //returns suit health

        int getPlanetsExpl();    //returns number of planets explored
        int getNumAccurateReport();  //returns number of habitable planets(accurate report)
        int getNumInaccurateReport();  //returns number of habitable planets(inaccurate report)

        int getHumanSaved();   //returns number of humans saved
        int getHumanPerished();  //returns number of humans perished


        //Setters*********
        void setPlayerName(string nName);   //sets player name
        
        //void setPlayerAmountMoney(int money); //sets the amount of money the user has
        void setChangeAmountMoney(int amountToAddSubtract_money);  //changes amount of money
        void setChangeAmountFuel(int amountToAddSubtract_fuel);   //changes gallons of fuel

        void setSuitLVL(int newLVL);    //changes space suit level

        void setNumMedKit(int amountToAddSubtract_MK);   //changes number of med kits
        void setNumLSaber(int amountToAddSubtract_LS);   //changes number of light sabers
        void setNumBlaster(int amountToAddSubtract_BLAST); //changes number of blasters
        void setNumBeamGun(int amountToAddSubtract_BG);  //changes number of beam guns
        void setTDevice(bool boughtDevice);  //changes t/f for translator device stored

        void setPlayerHealth(double percentAsDecimal_subtractOrAdd_P);  //changes player health
        void setSuitHealth(double percentAsDecimal_subtractOrAdd_S);  //changes suit health

        void setPlanetsExplored(int addToPlanets);  //count for numPlanet
                    
                            //report acuracy num setter
        void setAccurateRep(int addToAR);
        void setInaccurateRep(int addToIR);

                            //when report
        void setHumansSaved(int addHumansS);    //numhuuman save
        void setHumansPerished(int addHumansP); //numhuman dead
        
        void setCrewMates();    //usr chooses crew mates and asigns them to crew mate string[]
};

#endif