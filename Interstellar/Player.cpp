

/*
* define functions in player.h
**/

#include "player.h"
#include <fstream>
#include <vector>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iomanip>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;


int split1(string inputString, char seperator, string arr[], int size){
    int count = 0, sepCount = 0, index = 0, strLength = inputString.length();
    if (inputString == ""){
        return 0;
    }
    for (int i = 0; i < strLength; i++){
        int length_to_split = i - index;
        if (count < size){
            if (inputString[i] == seperator){ 
                arr[count] = inputString.substr(index, length_to_split);
                count++;
                sepCount++;
                index = i + 1;
            }
            if (i + 1 == strLength){
                arr[count] = inputString.substr(index, length_to_split + 1);
                count++;
            }
        }
        else{
            return -1;
        }
    }
    if (sepCount == 0){
        arr[0] = inputString;
        return 1;
    }
    return count;
}
 


Player::Player(){
    playerName = "";

    playerHealth = 1.00;
    suitHealth = 1.00;

    fuelInGal = 200000;
    amountMoney = 100000;
    spaceSuitLVL = 1;

    numMedKits = 0;
    numLightSaber = 1;
    numBlaster = 0;
    numBeamGun = 0;
    haveTranDevice = false;

    numPlanetsExplored = 0;
    numHabPlanets_Accurate = 0;
    numHabPlanets_Inaccurate = 0;
    humansSaved = 0;
    humansPerished = 0;

    waltTF = false;
}

void Player::resetSH(){
    suitHealth = 1.00;
}
 
string Player::getPlayerName(){                     //getter function for player name
    return playerName;
}

string Player::getSelectCrewNames(int index){       //getter function for crewmate names
    return crewMates[index];
}

int Player::getAmountFuel(){                //getter function for fuel in gallons
    return fuelInGal;
}

int Player::getAmountMoney(){              //getter function for amount of $
    return amountMoney;
}

int Player::getNumLSaber(){                //getter function for number of lightsabers
    return numLightSaber;
}

int Player::getSpaceSuitLevel(){           //getter function for space suit level
    return spaceSuitLVL;
}

int Player::getNumMedKit(){               //getter function for number of med kits
    return numMedKits;
}

int Player::getNumBlaster(){            //getter function for number of blasters
    return numBlaster;
}

int Player::getNumBeamGun(){             //getter function for number of beam guns
    return numBeamGun;
}

bool Player::checkTranDevice(){          //getter function for true/false translator device in inventory
    return haveTranDevice;
}

double Player::getPlayerH(){             //getter function for player health
    return playerHealth;
}

double Player::getSuitH(){              //getter function for suit health
    return suitHealth;
}

int Player::getPlanetsExpl(){           //getter function for number of planets explored
    return numPlanetsExplored;
}

int Player::getNumAccurateReport(){      //getter function for number of habitable planets (accurate)
    return numHabPlanets_Accurate;
}

int Player::getNumInaccurateReport(){     //getter function for number of habitable planets (inaccurate)
    return numHabPlanets_Inaccurate;
}

int Player::getHumanSaved(){             //getter function for number of humans saved
    return humansSaved;
}

int Player::getHumanPerished(){          //getter function for number of humans perished
    return humansPerished;
}




void Player::setPlayerName(string nName){       //setter function for player name
    playerName = nName;    
}

void Player::setChangeAmountFuel(int amountToAddSubtract_fuel){    //setter function for amount of fuel in gallons
    fuelInGal += amountToAddSubtract_fuel;
}

void Player::setChangeAmountMoney(int amountToAddSubtract_money){  //setter function for amount of money
    amountMoney = amountMoney + amountToAddSubtract_money;
}

void Player::setSuitLVL(int newLVL){              //setter function for space suit level
    spaceSuitLVL = newLVL;
}

void Player::setNumMedKit(int amountToAddSubtract_MK){   //setter function for number of med kits
    numMedKits += amountToAddSubtract_MK;
}

void Player::setNumLSaber(int amountToAddSubtract_LS){    //setter function for number of light sabers
    numLightSaber += amountToAddSubtract_LS;
}

void Player::setNumBlaster(int amountToAddSubtract_BLAST){   //setter function for number of blasters
    numBlaster += amountToAddSubtract_BLAST;
}

void Player::setNumBeamGun(int amountToAddSubtract_BG){     //setter function for number of beam guns
    numBeamGun += amountToAddSubtract_BG;
}

void Player::setTDevice(bool boughtDevice){    //setter function for true/false translator device in inventory
    haveTranDevice = boughtDevice;
}

void Player::setPlayerHealth(double percentAsDecimal_subtractOrAdd_P){    //setter function for player health
    playerHealth += percentAsDecimal_subtractOrAdd_P;
    //check if user has med kits and if so use when health bellow 60%
    if (playerHealth <= 0.6 && numMedKits > 0){
        playerHealth += 0.4;
        numMedKits--;
    }
    //if walter is selected then user will be brought back from dead once
    if(crewMates[0] == "Walter" || crewMates[1] == "Walter"){
        if (playerHealth <= 0.0099999 && waltTF == false){
            cout << "You died...";
            this_thread::sleep_for(chrono::seconds(1));
            cout << " LMAO, just kidding. Luckily your crewmate Walter used his magic to bring you back to life (Note, Walter can only use his powers once)." << endl;
            playerHealth = 1;
            waltTF = true;
        }
    }
}

void Player::setSuitHealth(double percentAsDecimal_subtractOrAdd_S){  //setter function for suit health
    //crew mate Nellie repair suit
    //check if in crew arr
    if (crewMates[1] == "Nellie" || crewMates[0] == "Nellie"){
        if(suitHealth > suitHealth + percentAsDecimal_subtractOrAdd_S){
            cout << "Lucky for you, Your crewmate Nellie is great at repairing space suits. you gained back 5 percent of your space suit's health.\n";
            suitHealth += 0.05;
        }
    }
    suitHealth += percentAsDecimal_subtractOrAdd_S;
}

void Player::setPlanetsExplored(int addToPlanets){   //setter function for number of planets explored
    numPlanetsExplored += addToPlanets;
}

void Player::setAccurateRep(int addToAR){       //setter function for number of habitable planets (accurate)
    numHabPlanets_Accurate += addToAR;
}

void Player::setInaccurateRep(int addToIR){     //setter function for number of habitable planets(inaccurate)
    numHabPlanets_Inaccurate += addToIR;
}

void Player::setHumansSaved(int addHumansS){   //setter function for number of humans saved
    humansSaved += addHumansS;
}

void Player::setHumansPerished(int addHumansP){  //setter function for number of humans perished
    humansPerished += addHumansP;
}

void Player::setCrewMates(){
    for (int i = 0; i < 60; i++){
        cout << endl;
    }
    // vector to hold all crewmate info
    vector<string> cMates_AllNames;
    vector<string> cMates_AllAbilities;
    
    // split line from file then add corresponding parts to vector for crewMates and their atributes
    string line1;
    ifstream input;   
    input.open("crewmates.txt");
    while (getline(input, line1)){
        string temp[2];
        split1(line1, ';', temp, 2); 
        cMates_AllNames.push_back(temp[0]);
        cMates_AllAbilities.push_back(temp[1]);
    }
    input.close();

    //same as above except for extra credit crew options
    string line2;
    ifstream ecInput;
    ecInput.open("crewmates_ec.txt");
    while (getline(ecInput, line2)){
        string temp[2];
        split1(line2, ';', temp, 2); 
        cMates_AllNames.push_back(temp[0]);
        cMates_AllAbilities.push_back(temp[1]);
    }
    ecInput.close();

    // print names and atributs in a list that pairs atribute to crewmate name
    cout << "Pick a crewmate to help you on your journey!\n";
    //index for printed option
    int idxVect = 1;
    // loop print crew mate and ability
    for (int i = 0; i < cMates_AllNames.size(); i++){
        cout << "\n" << idxVect << ". " << cMates_AllNames[i] << endl;
        cout << "Ability: " << cMates_AllAbilities[i] << endl;
        idxVect++;
    }
    //user makes choice
    int userChoice1;
    cin >> userChoice1;
    //input validation
    while (userChoice1 > cMates_AllNames.size() || userChoice1 <= 0){
        cout << "\nInvalid Input. Re-enter a number coresponding with the choices above.\n";
        cin >> userChoice1;
    }
    //index at vector starts at 0
    int nameVecIDX1 = userChoice1-1;
    //add user choice to crew mate arr
    crewMates[0] = cMates_AllNames[nameVecIDX1];

    for (int i = 0; i < 60; i++){
        cout << endl;
    }
    //tell user which crewmate confirmation
    cout << '\n' << '\n' << "\nGreat choice! " << cMates_AllNames[nameVecIDX1] << " will bring a lot to your team. Pick your last crewmate.\n";
    

    //delete from vector as to not select when picking second option
    cMates_AllNames.erase(cMates_AllNames.begin() + nameVecIDX1);
    cMates_AllAbilities.erase(cMates_AllAbilities.begin() + nameVecIDX1);


    //print again with new index and take out crewmate user selected last time
    int idxVect2 = 1;
    for (int i = 0; i < cMates_AllNames.size(); i++){
        cout << "\n" << idxVect2 << ". " << cMates_AllNames[i] << endl;
        cout << "Ability: " << cMates_AllAbilities[i] << endl;
        idxVect2++;
    }
    int userChoice2;
    cin >> userChoice2;
    while (userChoice2 > cMates_AllNames.size() || userChoice2 <= 0){
        cout << "\nInvalid Input. Re-enter a number coresponding with the choices above.\n";
        cin >> userChoice2;
    }

    //add second choice to arr
    int nameVecIDX2 = userChoice2-1;
    crewMates[1] = cMates_AllNames[nameVecIDX2];
    //message to user 
    cout << "\nYou have a very impressive crew. With your leadership, you will be sure to succeed.\n";
}


/*******Garbage version code********/  

// ---> previous partner that I ditched(zoe pekarek); hardly worked on code. Coppied both proffesors and TA on email directed at her. I mentioned she could keep whatever she had from my code. Mentioning in case I get flagged for having simmilar code.  

    // string line;
    // string line2;
    // int choice;
    // int choice2;
    // int idxOptionNum = 1;
    // cout << "Pick a crewmate to help you on your journey!\n"; 

    // while (getline(input, line)){ 
    //     string tempCrewMate[2];
    //     split(line, ";", tempCrewMate, 2); 
    //     cout << idxOptionNum << ". " << tempCrewMate[0] << endl;
    //     cout << "Ability: " << tempCrewMate[1] << endl;        
    //     idxOptionNum++;   
    // }
    // input.close();

    // while (getline(ecInput, line2)){ 
    //     string tempCrewMate[2];
    //     split(line2, ";", tempCrewMate, 2);                       
    //     cout << idxOptionNum << ". " << tempCrewMate[0] << endl; 
    //     cout << "Ability: " << tempCrewMate[1] << endl;      
    //     idxOptionNum++;   
    // }
    // ecInput.close(); 

    // cin >> choice;
    // while(choice < 1 || choice > idxOptionNum - 1){             
    //     cout << "Invalid input. Re-enter one of the above choicces" << endl;
    //     cin >> choice;
    // }
    
    // input.open("crewmates.txt");
    // line = "";               
    // int newIdx = 1;
    // while (getline(input, line)){      
    //     if (newIdx == choice){
    //         string temp[2];
    //         split(line, ";", temp, 2);    
    //         string temphold = temp[0];
    //         crewMates[0] = temphold;
    //     }
    //     else{
    //         newIdx++;
    //     }
    // }
    // input.close();
    // if (newIdx == 5){  
    //     ecInput.open("crewmates_ec.txt");                                      //put choice into crewmates array from 2nd file
    //     line2 = "";
    //     while(getline(ecInput, line2)){
    //         if (newIdx == choice){
    //             string temp[2];
    //             split(line2, ";", temp, 2);                    //splits each line at the first occurance of semicolon, sorts into array with length 2
    //             string temphold = temp[0];
    //             crewMates[0] = temphold;
    //         }
    //         else{
    //             newIdx++;
    //         }
    //     }
    //     ecInput.close();
    // }
    // cout << "Great choice! " << crewMates[0] << " will bring a lot to your team. Pick your last crewmate. " << endl;

    // input.open();
    // ecInput.open();
    // idxOptionNum = 1;
    // while (getline(input, line)){                                //printing first file choices
    //     if(idxOptionNum != choice){
    //         string tempCrewMate[2];
    //         split(line, ";", tempCrewMate, 2);
    //         cout << idxOptionNum << ". " << tempCrewMate[0] << endl;
    //         cout << "Ability: " << tempCrewMate[1] << endl;
    //     }
    //     idxOptionNum++;
    // }
    // line = "";                                                   //resets string holding each line
    // while (getline(ecInput, line)){                              //printing extra credit file choices
    //     if(idxOptionNum != choice){
    //         string tempCrewMate[2];
    //         split(line, ";", tempCrewMate, 2);
    //         cout << idxOptionNum << ". " << tempCrewMate[0] << endl;
    //         cout << "Ability: " << tempCrewMate[1] << endl;
    //     }
    //     idxOptionNum++;
    // }
    // cin >> choice2;
    // input.close();
    // ecInput.close();

    // input.open();
    // ecInput.open();
    // while(choice2 < 1 || choice2 > idxOptionNum-1){               //checking choice validity
    //     cout << "Invalid input." << endl;
    //     idxOptionNum = 1;
    //     line = "";                                                //resets string holding each line
    //     while (getline(input, line)){                                //printing first file choices
    //         if(idxOptionNum != choice){
    //             string tempCrewMate[2];
    //             split(line, ";", tempCrewMate, 2);
    //             cout << idxOptionNum << ". " << tempCrewMate[0] << endl;
    //             cout << "Ability: " << tempCrewMate[1] << endl;
    //         }
    //         idxOptionNum++;
    //     }
    //     line = "";                                                //resets string holding each line
    //     while (getline(ecInput, line)){                              //pritning extra credit file choices
    //         if(idxOptionNum != choice){
    //             string tempCrewMate[2];
    //             split(line, ";", tempCrewMate, 2);
    //             cout << idxOptionNum << ". " << tempCrewMate[0] << endl;
    //             cout << "Ability: " << tempCrewMate[1] << endl;
    //         }
    //         idxOptionNum++;
    //     }
    //     cin >> choice2;
    // }
    // input.close();
    // ecInput.close();
    
    // input.open();
    // line = "";                                                 //resets string holding each line
    // int newIdx = 1;
    // while (getline(input, line)){                               //put choice into crewMates array from 1st file
    //     if (newIdx == choice2){
    //         string temp[2];
    //         split(line, ";", temp, 2);
    //         string temphold = temp[0];
    //         crewMates[1] = temphold;
    //     }
    //     else{
    //         newIdx++;
    //     }
    // }
    // if (newIdx == 5){  
    //     ecInput.open();                                      //put choice into crewmates array from 2nd file
    //     line = "";                                        //resets string holding each line
    //     while(getline(ecInput, line)){
    //         if (newIdx == choice2){
    //             string temp[2];
    //             split(line, ";", temp, 2);
    //             string temphold = temp[0];
    //             crewMates[1] = temphold;
    //         }
    //         else{
    //             newIdx++;
    //         }
    //     }
    // }
    // cout << "You have a very impressive crew. With your leadership, you will be sure to succeed.\n";