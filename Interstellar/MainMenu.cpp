

// DRIVER FOR GAME CONTAINS INT MAIN

#include "friendlyAlien.h"
#include "player.h"
#include "hostileAlien.h"
#include "hiddenMap.h"
#include "LogBook.h"
#include <fstream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cstring>
#include <iomanip>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <iostream>

using namespace std;




/********************* helping functions used in int main *********************/

// SPLIT FUNCTION splits string and adds to array of designated size
int split(string inputString, char seperator, string arr[], int size){
    int count = 0, sepCount = 0, index = 0, strLength = inputString.length();
    //make sure string to split is not empty
    if (inputString == ""){
        return 0;
    }
    //iterate char by char through string
    for (int i = 0; i < strLength; i++){
        // split leangth is from i --> last idx plus 1
        int length_to_split = i - index;
        
        //make sure to stay in bounds of arr[]
        if (count < size){
            if (inputString[i] == seperator){                               //check for delimiter
                arr[count] = inputString.substr(index, length_to_split);
                count++;        //count added substr to arr[]
                sepCount++;     //count delimiters
                index = i + 1;  //idx set to be 1 after delimiter
            }
            if (i + 1 == strLength){    //last string no delimiter at end
                arr[count] = inputString.substr(index, length_to_split + 1);
                count++;
            }
        }
        else{   //could not perform split???
            return -1;
        }
    }
    if (sepCount == 0){     //no delimiters then add full string to arr[]
        arr[0] = inputString;
        return 1;
    }
    return count;  //return number of substr added to arr[]
}

/*****GETS & RETURNS STRING ARR WITH QUESTION AND ANSWERS FOR RANDOM TRIVIA AT SITE*****/
void askRand(string qAnd_a[]){
    //total 86 lines in question file (non inclusve of 0)
    int rIDX = rand() % 86 + 1;
    int count = 1;                  //count lines
    string questionLine[2];         //to hold parts after split
    
    //read txt file with getline
    string line;
    ifstream input;
    input.open("questions.txt");
    while (getline(input, line)){
        //only store question and answers for the line picked by rand above
        if (count == rIDX){
            split(line, ';', questionLine, 2);  //seperates question from 4 answers
        }
        count++;
    }

    //split answer options appart
    string temp = questionLine[1];
    split(temp, ',', qAnd_a, 4);

    //add question to the arrray
    qAnd_a[4] = questionLine[0];
}

/******PICKS RANDOM POSITION ON THE GAME MAP'S COORDINATE PLANE TO SPAWN ENTITY******/
//modifies intiger array[2] (holds row and col)
void randPosSpawn(int spawnRoCo[], int i){
    
    //int i is random 1 or 2 (bc when position for collum is spawned at 0 then row cannot be 0 so make 50 50 to decide which could possibly have 0 pos)
    //store coordinate position to int arr[]. position 0-11 or 1-11 depending on i is 1 or 2
    if(i == 2){
        spawnRoCo[0] = rand() % 11;
        spawnRoCo[1] = rand() % 11 + 1;
    }
    //oposite rand value min than above for row and col so that coordinate still completely rand while avoiding player initial position
    else if(i == 1){
        spawnRoCo[0] = rand() % 11 + 1;
        spawnRoCo[1] = rand() % 11;
    }
}

/******PRINT MAIN MENU AND CHOOSE OPTION FROM MENU******/
//prevent having to repetative copy paste in int main()
int printMainMenu(){
    int pick;   //user choice variable
    cout << "Select one:\n" << "1. Move\n" << "2. View status\n" << "3. View log book\n" << "4. Resource center\n" << "5. Report planet as habitable\n" << "6. Enter wormhole to next planet\n" << "7. Give up\n";
    cin >> pick;

    //input validation to make sure user enters valid number
    while (pick != 1 && pick != 2 && pick != 3 && pick != 4 && pick != 5 && pick != 6 && pick != 7){
        cout << "Invalid Input. Re-enter a valid number 1-7.\n" << " Select one:\n" << "1. Move\n" << "2. View status\n" << "3. View log book\n" << "4. Resource center\n" << "5. Report planet as habitable\n" << "6. Enter wormhole to next planet\n" << "7. Give up\n";
        cin >> pick;
    }
    return pick;  //returns the intiger choice to determine what to do from menu
}

/*****PRINT PERCENT BAR FOR VIEW STATUS (TAKES PERCENT 0-100)*****/
//must calculate % before calling function
void percentageBar(int pCent){
    //maximum 20 bars when percent is > 95

    //one bar every 5 percent
    for(int i = 0; i < (pCent/5); i++){
        cout << '=';    //equals looked better as a bar than underscore
    }
    //add extra bar to round up instead of down
    if (pCent % 5 > 0){  //remainder so that when pcent is multiple of 5 it does not add extra
        cout << '=';
    }
    //add space after the bar to print the percentage (make sure all percentages are same length away from left side of terminal) 
    if (pCent <= 100){
        for(int j = 0; j < 21-(pCent/5); j++){  //one extra space after the bar 21 instead of 20
            cout << ' ';
        }
    }
    //casewhere one of crew mates gives you additiional 200000 fuel = 133%
    else{
        cout << ' ';
    }
    cout << pCent << '%' << endl;   //print percentage and end line
}

/*****CALCULATE COST OF PLANET TRAVEL FOR GIVEN NUMBER OF PLANETS EXPLORED*****/
//helper and call function to do recursive calculation
int fuelcost_RecurHelper(int inputF, int idx){
    if (idx > 0){
        int answr = inputF * inputF / 1000000 + inputF;
        idx--;
        return fuelcost_RecurHelper(answr, idx);
    }
    else{
        return inputF;
    }
}
int fuelcost_Recur(int index){
    index--;                        //input is planet number currently so at p1 it should not recurse so always idx-1
    int initialF = 50000;           //initial cost of first travel
    return fuelcost_RecurHelper(initialF, index);  //recurse in helper function with fuel calculation takes perameter for (cost of first travel, numb times to recurse)
}

/*******COMPARE 2 STRINGS FOR ALPHABETICAL BUBBLE SORT*******/
//asigns int value to the comparison of 2 strings
int stringComparison(string t1, string t2){         
    //perameter are strings with first 6 char in planet name for position i and i+1 insite vector containig test file names = (pnames) + (.txt)

    //define char array with order of values being least to greatest (order to sort)
    char cArr[35] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    
    //iterate through first six letters of the planet name
    for(int i = 0; i < 6; i++){
        //intiger temps to hold value of t1 & t2 at [i]
        int te1;
        int te2;
        
        //iterate through cArr to check against input strings 
        for(int j = 0; j < 35; j++){
            //if the char from arr at j matches the char in string temp for given position i then set the parallel int temp to equal j
            if(t1[i] == cArr[j]){
                te1 = j;
            }
            if(t2[i] == cArr[j]){
                te2 = j;
            }
        }

        //compare both int
        if(te1 > te2){  //if first is bigger then the strings need to swap in the vector bc the first string is later in the set order made by cArr[35]={...}
            return 1;   //returns to break out of loop
        }
        if(te1 < te2){  //opposite as above(does not switch the two strings)
            return -1;  
        }
        //if char for temp1 or 2 [i] are equal than 'for loop' automaticly checks next letter
    }
    return 0; //when both strings are identicle function returns default of 0 (no swap unless return > 0)
}

//animate new rocket
// all animations are single stop motion parts
void animation(){
    for(int i = 0; i < 65; i++){
        cout << endl;
    }

    ifstream L2input;
    string L2linef;
    L2input.open("launch2.txt");
    while (getline(L2input, L2linef)){
        cout << L2linef << endl;
    }
    L2input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L3input;
    string L3linef;
    L3input.open("launch3.txt");
    while (getline(L3input, L3linef)){
        cout << L3linef << endl;
    }
    L3input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L4input;
    string L4linef;
    L4input.open("launch4.txt");
    while (getline(L4input, L4linef)){
        cout << L4linef << endl;
    }
    L4input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L5input;
    string L5linef;
    L5input.open("launch5.txt");
    while (getline(L5input, L5linef)){
        cout << L5linef << endl;
    }
    L5input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L6input;
    string L6linef;
    L6input.open("launch6.txt");
    while (getline(L6input, L6linef)){
        cout << L6linef << endl;
    }
    L6input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L7input;
    string L7linef;
    L7input.open("launch7.txt");
    while (getline(L7input, L7linef)){
        cout << L7linef << endl;
    }
    L7input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L8input;
    string L8linef;
    L8input.open("launch8.txt");
    while (getline(L8input, L8linef)){
        cout << L8linef << endl;
    }
    L8input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L9input;
    string L9linef;
    L9input.open("launch9.txt");
    while (getline(L9input, L9linef)){
        cout << L9linef << endl;
    }
    L9input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L10input;
    string L10linef;
    L10input.open("launch10.txt");
    while (getline(L10input, L10linef)){
        cout << L10linef << endl;
    }
    L10input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L11input;
    string L11linef;
    L11input.open("launch11.txt");
    while (getline(L11input, L11linef)){
        cout << L11linef << endl;
    }
    L11input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L12input;
    string L12linef;
    L12input.open("launch12.txt");
    while (getline(L12input, L12linef)){
        cout << L12linef << endl;
    }
    L12input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L13input;
    string L13linef;
    L13input.open("launch13.txt");
    while (getline(L13input, L13linef)){
        cout << L13linef << endl;
    }
    L13input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L14input;
    string L14linef;
    L14input.open("launch14.txt");
    while (getline(L14input, L14linef)){
        cout << L14linef << endl;
    }
    L14input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L15input;
    string L15linef;
    L15input.open("launch15.txt");
    while (getline(L15input, L15linef)){
        cout << L15linef << endl;
    }
    L15input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L16input;
    string L16linef;
    L16input.open("launch16.txt");
    while (getline(L16input, L16linef)){
        cout << L16linef << endl;
    }
    L16input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L17input;
    string L17linef;
    L17input.open("launch17.txt");
    while (getline(L17input, L17linef)){
        cout << L17linef << endl;
    }
    L17input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L18input;
    string L18linef;
    L18input.open("launch18.txt");
    while (getline(L18input, L18linef)){
        cout << L18linef << endl;
    }
    L18input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L19input;
    string L19linef;
    L19input.open("launch19.txt");
    while (getline(L19input, L19linef)){
        cout << L19linef << endl;
    }
    L19input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L20input;
    string L20linef;
    L20input.open("launch20.txt");
    while (getline(L20input, L20linef)){
        cout << L20linef << endl;
    }
    L20input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L21input;
    string L21linef;
    L21input.open("launch21.txt");
    while (getline(L21input, L21linef)){
        cout << L21linef << endl;
    }
    L21input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L22input;
    string L22linef;
    L22input.open("launch22.txt");
    while (getline(L22input, L22linef)){
        cout << L22linef << endl;
    }
    L22input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L23input;
    string L23linef;
    L23input.open("launch23.txt");
    while (getline(L23input, L23linef)){
        cout << L23linef << endl;
    }
    L23input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L24input;
    string L24linef;
    L24input.open("launch24.txt");
    while (getline(L24input, L24linef)){
        cout << L24linef << endl;
    }
    L24input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L25input;
    string L25linef;
    L25input.open("launch25.txt");
    while (getline(L25input, L25linef)){
        cout << L25linef << endl;
    }
    L25input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L26input;
    string L26linef;
    L26input.open("launch26.txt");
    while (getline(L26input, L26linef)){
        cout << L26linef << endl;
    }
    L26input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L27input;
    string L27linef;
    L27input.open("launch27.txt");
    while (getline(L27input, L27linef)){
        cout << L27linef << endl;
    }
    L27input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L28input;
    string L28linef;
    L28input.open("launch28.txt");
    while (getline(L28input, L28linef)){
        cout << L28linef << endl;
    }
    L28input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L29input;
    string L29linef;
    L29input.open("launch29.txt");
    while (getline(L29input, L29linef)){
        cout << L29linef << endl;
    }
    L29input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L30input;
    string L30linef;
    L30input.open("launch30.txt");
    while (getline(L30input, L30linef)){
        cout << L30linef << endl;
    }
    L30input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L31input;
    string L31linef;
    L31input.open("launch31.txt");
    while (getline(L31input, L31linef)){
        cout << L31linef << endl;
    }
    L31input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L32input;
    string L32linef;
    L32input.open("launch32.txt");
    while (getline(L32input, L32linef)){
        cout << L32linef << endl;
    }
    L32input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L33input;
    string L33linef;
    L33input.open("launch33.txt");
    while (getline(L33input, L33linef)){
        cout << L33linef << endl;
    }
    L33input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L34input;
    string L34linef;
    L34input.open("launch34.txt");
    while (getline(L34input, L34linef)){
        cout << L34linef << endl;
    }
    L34input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L35input;
    string L35linef;
    L35input.open("launch35.txt");
    while (getline(L35input, L35linef)){
        cout << L35linef << endl;
    }
    L35input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L36input;
    string L36linef;
    L36input.open("launch36.txt");
    while (getline(L36input, L36linef)){
        cout << L36linef << endl;
    }
    L36input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L37input;
    string L37linef;
    L37input.open("launch37.txt");
    while (getline(L37input, L37linef)){
        cout << L37linef << endl;
    }
    L37input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L38input;
    string L38linef;
    L38input.open("launch38.txt");
    while (getline(L38input, L38linef)){
        cout << L38linef << endl;
    }
    L38input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L39input;
    string L39linef;
    L39input.open("launch39.txt");
    while (getline(L39input, L39linef)){
        cout << L39linef << endl;
    }
    L39input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L40input;
    string L40linef;
    L40input.open("launch40.txt");
    while (getline(L40input, L40linef)){
        cout << L40linef << endl;
    }
    L40input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L41input;
    string L41linef;
    L41input.open("launch41.txt");
    while (getline(L41input, L41linef)){
        cout << L41linef << endl;
    }
    L41input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L42input;
    string L42linef;
    L42input.open("launch42.txt");
    while (getline(L42input, L42linef)){
        cout << L42linef << endl;
    }
    L42input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L43input;
    string L43linef;
    L43input.open("launch43.txt");
    while (getline(L43input, L43linef)){
        cout << L43linef << endl;
    }
    L43input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L44input;
    string L44linef;
    L44input.open("launch44.txt");
    while (getline(L44input, L44linef)){
        cout << L44linef << endl;
    }
    L44input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L45input;
    string L45linef;
    L45input.open("launch45.txt");
    while (getline(L45input, L45linef)){
        cout << L45linef << endl;
    }
    L45input.close();
    for (int j =  0; j < 1; j++){
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(200));
        for(int i = 0; i < 65; i++){
            cout << endl;
        }
    }

    ifstream L46input;
    string L46linef;
    L46input.open("launch46.txt");
    while (getline(L46input, L46linef)){
        cout << L46linef << endl;
    }
    L46input.close();
}






//int main() COMPILE/RUN INTERSTELER GAME
int main(){
    //**************************** PHASE 1 OF GAME ****************************//

    //set srand seed once
    srand(time(NULL));

    //initialize objects in game
    Player user = Player();
    hiddenMap gameMap = hiddenMap();
    Lbook nPage = Lbook();
    FAlien npc = FAlien();
    
    //generate values for planet rand traits stored in logbook
    nPage.setP_Name();
    nPage.setP_Diameter();
    nPage.setCarry_Cap();
    
    //vector to store all text file names. file name is planet name + .txt (textfiles created for planet if planet is reported. files contain logbook pages for reported planets)
    vector<string> PN_save;

    for (int i = 0; i < 60; i++){
        cout << endl;
    }

    //gamename print tittle
    string s1rng = "     dBP  dBBBBb dBBBBBBP dBBBP dBBBBBb ,dBBBBP dBBBBBBP dBBBP  dBP    dBP dBBBBBb   dBBBBBb \n";
          s1rng += "             dBP                    dBP BB                                      BB       dBP \n";
          s1rng += "   dBP  dBP dBP   dBP   dBBBP   dBBBBb  'BBBBb   dBP   dBBBP  dBP    dBP    dBP BB   dBBBBb  \n";
          s1rng += "  dBP  dBP dBP   dBP   dBP     dBP  BP     dBP  dBP   dBP    dBP    dBP    dBP  BB  dBP  BP  \n";
          s1rng += " dBP  dBP dBP   dBP   dBBBBP  dBP  dB' dBBBBP  dBP   dBBBBP dBBBBP dBBBBP dBBBBBBB dBP  dB'  \n";

    for (int i = 0; i < s1rng.length(); i++){
        cout << "\033[34m" << s1rng[i];
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(9));
    }
    
    for (int i = 0; i < 4; i++){
        cout << endl;
    }
    //Lettering order initially generated at --> http://patorjk.com/software/taag/#p=display&f=Trek&t=Interstellar
    //had to reformat individually

    cout << "\033[0mHit ENTER to begin game."<< endl;  //        <----- use this function a lot to have user press space bar before continuing
    while(true){                    //until user hits ENTER the loop will repeat
        if (cin.get() == '\n'){     //cin.get() doesn't ignore white spaces whereas regular cin will. additionally no variable needs to be declared bc cin.get() grabs char from array usually
            break;                  //break out of while loop when condition is met               usually an extraction operator overlooks ' ' or '\n' or 'tab'
        }
    }

    //get username from user
    cout << "\nWELCOME TO BASE ASTRONAUT. THE WHOLE OF HUMANITY IS DEPENDING ON YOU TO FIND A NEW PLANET FOR HUMANS TO CALL HOME.\n" << "Please enter your astronaut code name:" << endl;
    string astroName;
    cin >> astroName;
    user.setPlayerName(astroName);
    //ask user for input name then use setter in player class ^^
    
    //function in Player.cpp (asks user to select crewmates)
    user.setCrewMates();

    //getSelectCrewNames takes in index 1 or 0 for index in crew mats string arr which holds name of both crewmates
    //bobby gives user LVL 3 suit so if user selected bobby then set suit level to 3 
    if (user.getSelectCrewNames(0) == "Bobby" || user.getSelectCrewNames(1) == "Bobby"){
        cout << "Your crewmate Bobby is generous enough to let you borrow his LVL 3 space suit free of charge.\n";
        user.setSuitLVL(3);
    }
    //add another 200,000 gal fuel if millie is one of users crewmates because millie doubles your fuel at the start of the game
    if (user.getSelectCrewNames(0) == "Millie" || user.getSelectCrewNames(1) == "Millie"){
        cout << "Your crewmate Millie has an excess storage of fuel and starts you off with double the fuel (133%).\n";
        user.setChangeAmountFuel(200000);
    }

    //info for user abt move
    cout << "\nHumans back on Earth will be watching your every move! Billionaire Warren Buffet stated that he will invest $50 in every move you make.\n";

    //wait for user to input enter
    cout << "Hit ENTER to continue." << endl;
    cin.ignore();         //was recognizing input as the last \n in program on line 266 (happens a lot in program; will come up again)
    while(true){
        if (cin.get() == '\n'){
            break;
        }
    }
    
    //initialize variables used to calculate score (variables will be incremented throughout game)
    double total_HumansToSave = 8000.0;
    double deadH = 0.0;
    double aliveH = 0.0;
    int noREP_NH = 0;

    // print info about RC
    string lineRC;
    ifstream inputRC;   
    for (int i = 0; i < 60; i++){
        cout << endl;
    }
    inputRC.open("resource_center_info.txt");
    while(getline(inputRC, lineRC)){
        cout << lineRC << endl;
    }

    //user has to hit enter
    cout << "Hit ENTER to continue." << endl;
    while(true){
        if (cin.get() == '\n'){
            break;
        }
    }


    bool checkIfleave = false;      //leave RC menu when = true
    // initial resource center menu for pre-takeoff 
    while(checkIfleave == false){
        vector<string> choiceVect;  //vector to hold menu options that are going to print
        int pChoice;    //variable for user menu choice
        int index = 1;  //index number of options printed to menu/numbering of menu left side
        cout << "Which item do you wish to buy?\n";
        // each menu line is printed as:  "index. menuItemOption"

        //num weapons < 2 to be able to print on menu or buy
        if ((user.getNumLSaber() + user.getNumBlaster() + user.getNumBeamGun()) < 2){
            cout << index << ". Weapon\n";  
            index++;
            choiceVect.push_back("weapon");
        }
        //user must not already own Translator for user to be able to print it on menu or buy 
        if (user.checkTranDevice() == false){
            cout << index << ". Translator\n";
            index++;
            choiceVect.push_back("translator");
        }
        //make sure suit level is less than 5 to be able to print on menu or buy
        if (user.getSpaceSuitLevel() < 5){
            cout << index << ". Space suit\n";
            index++;
            choiceVect.push_back("spacesuit");
        }
        //check & make sure user has less than 5 medkit to be able to print on menu or buy
        if (user.getNumMedKit() < 5){
            cout << index << ". Medical Kits\n";
            index++;
            choiceVect.push_back("medkits");
        }
        //only print option if user has less than 300000 gal fuel
        if (user.getAmountFuel() < 300000){
            cout << index << ". Fuel\n";
            index++;
            choiceVect.push_back("fuel");
        }
        //Leave option always on menu
        cout << index << ". Leave" << endl;
        choiceVect.push_back("leave");
        cin >> pChoice;
        
        // input validation (make sure user picked an option on menu)
        while (pChoice <= 0 || pChoice > index){
            //RePrint menu options and tell user Invalid input
            cout << "Invalid input.\nWhich item do you wish to buy?\n";
            index = 1;
            if ((user.getNumLSaber() + user.getNumBlaster() + user.getNumBeamGun()) < 2){
                cout << index << ". Weapon\n";
                index++;
            }
            if (user.checkTranDevice() == false){
                cout << index << ". Translator\n";
                index++;
            }
            if (user.getSpaceSuitLevel() < 5){
                cout << index << ". Space suit\n";
                index++;
            }
            if (user.getNumMedKit() < 5){
                cout << index << ". Medical Kits\n";
                index++;
            }
            if (user.getAmountFuel() < 300000){
                cout << index << ". Fuel\n";
                index++;
            }
            cout << index << ". Leave\n";
            //user input again than check again with while loop
            cin >> pChoice;
        }

        string selectedFromMenu = choiceVect[pChoice-1];    //menu option strng is gotten from choice vector and compared to options bellow
        int dolAmountC = 0;                                 //holds amount money before purchase in sub menus
        int curAmount = user.getAmountMoney();              //amount money user has before purchase

     // WEAPON BUY MENU
        if (selectedFromMenu == "weapon"){
            int whichWeapon;    //pick from weapons menu
            int numWeapon;      //number of selected weapon from weapons menu
            bool quit = true;   //leave weapon menu using while true and change if leave
            while (quit){
                int totalWeapons = user.getNumLSaber() + user.getNumBlaster() + user.getNumBeamGun();       //after every pick reset keep updated total weapons count

                //gives user options for weapons to buy
                int whichWeapon;
                cout << "Which weapon type do you wish to buy?\n1. Light Saber\n2. Blaster\n3. Beam Gun\n4. Go back to menu\n";
                cin >> whichWeapon;
                //check valid input
                while(whichWeapon > 4 || whichWeapon < 0){
                    cout << "Re-enter a valid option 1 through 4\n";
                    cin >> whichWeapon;
                }

                //give price for each weapon if selected and ask how many of each weapon the user would like to buy
                //input validation of check total weapons 

                //buy lightt saber
                if (whichWeapon == 1){
                    cout << "How many Light Sabers do you want ($1000 each)?\n";
                    cin >> numWeapon;
                    // input validation
                    while (totalWeapons + numWeapon > 2 || numWeapon < 0){
                        cout << "You cannot enter negative numbers or have more than two weapons at the same time, re-enter a valid quantity.\n";
                        cin >> numWeapon;
                    }
                    // if enough money
                    if (curAmount - dolAmountC >= 1000*numWeapon){
                        user.setNumLSaber(numWeapon);                   //change number of the selected weapon to user input amount
                        dolAmountC += 1000*numWeapon;                   //calc $ after purchase
                        int answ = curAmount - dolAmountC;              //calc $ after purchase
                        cout << "You have $" << answ << " left.\n";
                    }
                    //not enough $
                    else{
                        cout << "You do not have enough money for that.\n";
                    }
                }
                //buy Blaster
                if (whichWeapon == 2){
                    cout << "How many Blasters do you want ($2000 each)?\n";
                    cin >> numWeapon;
                    //input validation.
                    while (totalWeapons + numWeapon > 2 || numWeapon < 0){
                        cout << "You cannot enter negative numbers or have more than two weapons at the same time, re-enter a valid quantity.\n";
                        cin >> numWeapon;
                    }
                    //user has suficient funds
                    if (curAmount - dolAmountC >= 2000*numWeapon){
                        user.setNumBlaster(numWeapon);             //change number of the selected weapon to user input amount
                        dolAmountC += 2000*numWeapon;              //calc $ after purchase
                        int answ = curAmount - dolAmountC;         // ^^
                        cout << "You have $" << answ << " left.\n";
                    }
                    //not enough $
                    else{
                        cout << "You do not have enough money for that.\n";
                    }
                }
                //buy Beam Gun
                if (whichWeapon == 3){
                    cout << "How many Beam Guns do you want ($5000 each)?\n";
                    cin >> numWeapon;
                    while (totalWeapons + numWeapon > 2 || numWeapon < 0){
                        cout << "You cannot enter negative numbers or have more than two weapons at the same time, re-enter a valid quantity.\n";
                        cin >> numWeapon;
                    }
                    //calculate cost if check enough $ is enough
                    if (curAmount - dolAmountC >= 5000*numWeapon){
                        user.setNumBeamGun(numWeapon);             //change number of the selected weapon to user input amount
                        dolAmountC += 5000*numWeapon;              //calc $ after purchase
                        int answ = curAmount - dolAmountC;         // ^^
                        cout << "You have $" << answ << " left.\n";
                    }
                    //not enough $
                    else{
                        cout << "You do not have enough money for that.\n";
                    }
                }

                //back to main menu
                if (whichWeapon == 4){
                    user.setChangeAmountMoney(-dolAmountC);     //change amount of money with variable for total spent
                    cout << "You now have $" << user.getAmountMoney() << " remaining.\n";   //tell user amount left
                    curAmount = user.getAmountMoney();  //re-set to current money with getter
                    quit = false;   //exit loop condition changed
                }
            }
        }
        
     // TRANSLATOR DEVICE PURCHASE
        else if(selectedFromMenu == "translator"){
            //prompt user to answer whether they want a translator or not
            //inform user of cost
            int yesNo;
            cout << "Would you like to purchase a translator device ($5000)? 1 for yes, 0 for no.\n";
            cin >> yesNo;
            // input validation, make sure 1 or 0 else say invalid and reprompt
            while (yesNo != 0 && yesNo != 1){
                cout << "Re-enter a valid input: 1 for yes, 0 for no.\n";
                cin >> yesNo;
            }
            //user wants to purchase
            if (yesNo == 1){
                //check to make sure user has enough money
                if (user.getAmountMoney() >= 5000){             // user has money
                    user.setTDevice(true);                      // give user translator device
                    user.setChangeAmountMoney(-5000);           // deduct cost of device from current amount money that user has
                    //tell user their current balance
                    cout << "You have $" << user.getAmountMoney() << " left.\n";
                }
                //user does not have enough
                else{                                                                                                                                                                                                                                                                                                                                                                                                                           //Maybe you wandered here randomly, or maybe you saw the clue at the bottom. Either way: You Just 'LOST THE GAME' https://en.wikipedia.org/wiki/The_Game_(mind_game) {sorry if this is your first time playing...}
                    //tell user that they are short money    
                    cout << "You do not have enough money for that.\n";
                    cout << "You have $" << user.getAmountMoney() << " left.\n";
                }
            }
            //no longer wants to make purchase
            else if (yesNo == 0){
                //tell user that they are going back to big rc menu & money left
                cout << "You have $" << user.getAmountMoney() << " left. You are now being redirect to the main Resource Center menu.\n";
            }
        }
        
     // UPGRADE SUIT
        else if (selectedFromMenu == "spacesuit"){
            int newGradeChoice;     //choice for upgrade that user 
            vector<int> ssVect;     //suits that can be bought/upgraded to

            //decide upgrade or not
            cout << "Which would you like to upgrade your spacesuit to? The space suit health will be set to 100%.\n";
            int index = 1; //for numbered list
            for (int i = user.getSpaceSuitLevel()+1; i < 6; i++){
                ssVect.push_back(i);        //put levels to buy into int vector
                cout << index << ". Space suit grade " << i << " costs $" << 5000*index << endl;   //numOnList. Space suit grade i costs $(5000*index)       pay difference:price of suit is not constant:price/suitLVL is constant
                index++; //add so that next number in list is not same
            }
            cin >> newGradeChoice;
            //INPUT VALIDATION
            while (newGradeChoice > index-1 || newGradeChoice <= 0){        //must stay in bounds of printed index
                cout << "Invalid input. Make sure to enter a number between 1 and " << index-1 << endl;
                cin >> newGradeChoice;
            }
            //check if user has enough money to make purchase
            if (newGradeChoice*5000 <= user.getAmountMoney()){
                int gradeLVL_Holder = ssVect[newGradeChoice-1]; //get level for menu choice made
                user.setSuitLVL(gradeLVL_Holder);   //change grade of suit
                user.resetSH();              //sets suit health to be full
                user.setChangeAmountMoney(-(5000*newGradeChoice));          //deduct cost from user money balance

                //confirm suit upgrade by telling user how much money they have left
                cout << "Your spacesuit is now a grade " << newGradeChoice << '.' << endl;
                cout << "You have $" << user.getAmountMoney() << " left.\n";
            }
            //user balance is too low
            else{
                cout << "You do not have enough money for that.\n";
                cout << "You have $" << user.getAmountMoney() << " left.\n";
            }
        }
        
     // PURCHASE MEDKITS
        else if (selectedFromMenu == "medkits"){
            int numMK;      // # of medkits user wants to buy
            //asks user for a number of MK, tells tham how may in inventory, and the max number they can have in their inventory
            cout << "How many medical kits would you like to purchase ($2000 each)? You currently have " << user.getNumMedKit() << ", and at any time can at most have 5.\n";
            cin >> numMK;
            // check input validation
            while (numMK+user.getNumMedKit() > 5 || numMK < 0){
                //inform user of Invalid input and tell the user perameters
                cout << "Invalid input. You cannot have more than 5 medkits and you cannot enter numbers less than 0. Re-enter a valid quantity.\n";
                cin >> numMK;
            }
            // make sure user has enough money
            if (numMK*2000 <= user.getAmountMoney()){
                user.setNumMedKit(numMK);           //give user # of medkits requested
                user.setChangeAmountMoney(-(2000*numMK));   //charge user balance
                cout << "You have $" << user.getAmountMoney() << " left.\n";    //tell user current balance
            }
            //not enough money
            else{
                //tell user purchase canceled due to insuficient funds and telll them current ballance
                cout << "You do not have enough money for that.\n";
                cout << "You have $" << user.getAmountMoney() << " left.\n";
            }
        }
        
     // PURCHASE FUEL
        else if (selectedFromMenu == "fuel"){
            int ammountFuelInput;   //store user input
            //tell user curent fuel in gal, max amount, price per unit galons, and input perameters
            cout << "You have " << user.getAmountFuel() << " gallons of fuel. You may only purchase up to a total of 300k gallons of fuel (current fuel + amount to buy).\nHow many gallons would you like to purchase ($1000 per 10,000 gallons)? Input must be in multiples of 10,000.\n";
            cin >> ammountFuelInput;
            //input validation make
            while (ammountFuelInput%10000!=0 || user.getAmountFuel()+ammountFuelInput > 300000){
                //ask user to enter new quantity when input does not follow perameters
                cout << "Input must be in multiples of 10,000 and cannot add up to exceed 300k gallons when added to your current amount. Re-enter a valid quantity.\n";
                cin >> ammountFuelInput;
            }
            // make sure that user has enough money for selected amount fuel
            if (ammountFuelInput/10 <= user.getAmountMoney()){
                user.setChangeAmountFuel(ammountFuelInput);
                user.setChangeAmountMoney(-(ammountFuelInput/10));
                cout << "You have $" << user.getAmountMoney() << " left.\n";
            }
            //user balance too low
            else{
                //tell user that too little ballance and also tell user their current ballance
                cout << "You do not have enough money for that.\n";
                cout << "You have $" << user.getAmountMoney() << " left.\n";
            }
        }

     // LEAVE MENU
        else if(selectedFromMenu == "leave"){
            checkIfleave = true;                //set the looping bool to equal true so that while(bool = is no longer false)
        }
    }

    cout << "\nGreat purchase, Elon is sending a Cybertruck with your supplies to you right now.\n" << "Hit ENTER when you are ready to start the launch sequence and explore your first planet in SPACE!\n";
    cin.ignore();
    while(true){
        if (cin.get() == '\n'){
            break;
        }
    }


    for (int i = 0; i < 60; i++){
        cout << endl;
    }

    //launch sequence 
    //count dount
    cout << "T-10 seconds till liftoff..." << endl;
    
    this_thread::sleep_for(chrono::seconds(1));
    // sleep_for() function pauses the current thread for a rel_time (real time) span.              After this time span has passed, the thread will resume its execution.
    // Current thread being paused in this case is MainMenu.cpp          I had to use endl and not \n because cout<< is a stream buffer (operation for reading and writing) and \n is a const char whereas endl clears any buffer the same way flush does.
    // seconds(1) makes a duration type object.                     Duration objects measure time (conveniently many measurements are in rel_tim = time the same way as a 60 min clock)
    // Duration object definitions are found inside of the <chrono> header/libreary which is a time library.

    for (int i = 10; i > 0; i--){
        // when the countdown reaches 6 print "Main engine start" along with a T- i
        if (i == 6){   
            cout << "T-"<< i << endl;
            cout << "Main engine start" << endl;
        }
        else{
            cout << "T-" << i << endl;
        }
        
        this_thread::sleep_for(chrono::seconds(1));     // freezes MainMenu.cpp for --> seconds(number of seconds to pause for) then counts again while i != 0
    }
    cout << "Solid Ignition and LIFTOFF!" << endl;

    animation();

    // //print liftoff txt file
    // cout << '\n';

    // ifstream L1input;
    // string L1linef;
    // L1input.open("launch1.txt");
    // while (getline(L1input, L1linef)){
    //     cout << L1linef << endl;
    // }
    // L1input.close();
    // for(int i = 0; i < 65; i++){
    //     cout << "       ." << endl;
    //     cout.flush();
    //     this_thread::sleep_for(chrono::milliseconds(35));
    // }

    this_thread::sleep_for(chrono::seconds(3));
    cout << '\n';

    //ask user to hit enter to continue game
    cout << "You Are Now In Space.\nHit ENTER to start exploring your first planet." << endl;
    while(true){
        if (cin.get() == '\n'){
            break;
        }
    } 





    //********************* MAIN MENU LOOP - PHASE 2 *********************//
    // spawn entities on map after resetting map
    // add to planet count (0 + 1 bc first planet)
    // subtract fuel cost of travel

    // spawn NPC
    int spawnAnyPos[2]; //position holder int arr
    if ((rand() % 2 + 1) == 1){     // 50% spawn rate for alien on map
        int x = rand() % 2 + 1;     //pick 1 or 2 for rand spawn func
        randPosSpawn(spawnAnyPos, x);       //returns array with positions for row & col
        gameMap.spawnNPC(spawnAnyPos[0], spawnAnyPos[1]);   //spawn the NPC (no test because map is empty and pos generator never spawns on player initial pos)
    }

    /** 
     * 1 is water
     * 2 is oxygen
     * 3 is fertile soil
     * 4 is bad atmosphere
     * 5 is toxic gas
     * 6 is extreme temperatures
    **/
    // spawn site
    for (int i = 0; i < 7; i++){    //spawn 7 sites with for loop
        int spawnAnyPos1[2];    //position holder int arr
        int sTrait = rand() % 6 + 1;    //random trait out of 6 traits
        int oneInTwo = rand() % 2 + 1;  //pick 1 or 2 for rand spawn func
        randPosSpawn(spawnAnyPos1, oneInTwo);   //returns array with positions for row & col
        //asign 2 ints to values in arr
        int x = spawnAnyPos1[0];
        int y = spawnAnyPos1[1];
        //check if spawn is free space ---> if false then redo rand pos func to get new values in arr
        while(gameMap.isFreeSpace(spawnAnyPos1[0], spawnAnyPos1[1]) == false){
            oneInTwo = rand() % 2 + 1;
            randPosSpawn(spawnAnyPos1, oneInTwo);
        }
        //use spawnsite func to place on map
        gameMap.spawnSite(spawnAnyPos1[0], spawnAnyPos1[1], sTrait);
    }

    /**
     * 1 is alien attack
     * 2 is extreme weather event
     * 3 is fall into crater
     * 4 is space sickness
    **/
    // spawn misfortunes
    int totalAmountMisf = rand() % 8 + 4;       //decides total number of misforturnes spawned between 8-4
    //at least 2 misf have to be a hostile alien
    //totalAmountMisf-2 bc 2 forsure hostile alien
    for (int i = 0; i < totalAmountMisf-2; i++){
        int spawnAnyPos2[2];                    //position holder int arr
        int oneInTwo = rand() % 2 + 1;          //pick 1 or 2 for rand spawn func
        randPosSpawn(spawnAnyPos2, oneInTwo);   //returns array with positions for row & col
        //check if spawn is free space ---> if false then redo rand pos func to get new values in arr
        while(gameMap.isFreeSpace(spawnAnyPos2[0], spawnAnyPos2[1]) == false){  
            oneInTwo = rand() % 2 + 1;
            randPosSpawn(spawnAnyPos2, oneInTwo);
        }
        int misf = rand() % 4 + 1;      //pick rand misf type to spawn
        gameMap.spawnMisfortune(spawnAnyPos2[0], spawnAnyPos2[1], misf);    //spawn misf that is a random misf type
    }
    //spawn min 2 hostile aliens
    for (int i = 0; i < 2; i++){
        int spawnAnyPos3[2];                //position holder int arr
        int oneInTwo = rand() % 2 + 1;          //pick 1 or 2 for rand spawn func
        randPosSpawn(spawnAnyPos3, oneInTwo);       //returns array with positions for row & col
        //check if spawn is free space ---> if false then redo rand pos func to get new values in arr
        while(gameMap.isFreeSpace(spawnAnyPos3[0], spawnAnyPos3[1]) == false){
            oneInTwo = rand() % 2 + 1;
            randPosSpawn(spawnAnyPos3, oneInTwo);
        }
        gameMap.spawnMisfortune(spawnAnyPos3[0], spawnAnyPos3[1], 1); //spawn misf predetermined type
    }

    //PREPARE TO GO INTO MAIN GAME LOOP (planet generation and conditions need to be set)

    user.setPlanetsExplored(1);     //go to first planet add to num planets
    int firstLaunch = fuelcost_Recur(1);    //first launch is 50,000 from recurse
    user.setChangeAmountFuel(-firstLaunch);     //take away from user ammount fuel

    //when one of crewmates is Sherry user will get back 10 percent used fuel
    int saved = firstLaunch/10;     
    if (user.getSelectCrewNames(0) == "Sherry" || user.getSelectCrewNames(1) == "Sherry"){
        cout << "Your master Pilot Sherry saved you " << saved << " fuel on his way to this planet (fuelused * 0.9).\n\n";
        user.setChangeAmountFuel(saved);
    }

    //store position of sites so that if return to same site cant get more traits. clears variables so that values can be added
    int indexSite = 0;
    int site_VisitAgain[7][2];
    for (int i = 0; i < 7; i++){
        site_VisitAgain[i][0] = -1;
        site_VisitAgain[i][1] = -1;
    }

    /***********LOOP TO KEEP PLAYER IN GAME UNTIL CERTAIN CONDITIONS***********/
    bool pReported = false;     //if reported there is a different game path for wormhole
    bool ENDgameM = false;      //ends while loop that keeps game running 

    // TAKES MENU OPTIONS AND RUNS MAIN GAME CREATION OF MAPS EXCT...
    while(ENDgameM == false && user.getSuitH() > 0.009999 && user.getPlayerH() > 0.009999){
        for (int i = 0; i < 60; i++){
            cout << endl;
        }

        //displays map, fresh variable for main menu choice
        gameMap.displayMap();
        int userChoiceMM;
    
        //print main menu and asign choice to fresh variable userChoiceMM
        userChoiceMM = printMainMenu();
        

        /****MOVE ON MAP****/
        if (userChoiceMM == 1){

            //ask for user input wasd to move
            cout << "Enter one of the following to move in the indicated direction on the map:\n" << "w. up\n" << "s. down\n" << "d. right\n" << "a. left\n";
            char movePick;
            cin >> movePick;
            
            //check if the move is on map and valid char input wasd
            bool moveTF = gameMap.executeMove(movePick);
            //check valid input
            while (moveTF == false){
                //prompt user to re-enter value for move until valid move is entered
                cout << "Re-enter input to be within bounds of the map and as one of the given options above (wasd).\n";
                cin >> movePick;
                moveTF = gameMap.executeMove(movePick);
            }

            //gain $50 on each move
            cout << "Warren Buffet direct diposited $50 into your checking acount for your progress.\nYou will be able to access the money instantly (faster than standard 3-4 business day transfers).\n";
            user.setChangeAmountMoney(50);
            // crewmate Mark gives and additional $100 on each move
            if (user.getSelectCrewNames(0) == "Mark" || user.getSelectCrewNames(1) == "Mark"){
                // crewmate Mark gives and additional $100 on each move
                // tell user money recieved
                cout << "Your crewmate Mark, gifted you an additional $100 for protecting him your team's journey.\n\n";
                user.setChangeAmountMoney(50);
            }


            /*-----------if moved to NPC-----------*/
            if (gameMap.isNPCLocation() == true){
                //print site txt file
                cout << "\033[32m" << '\n';
                ifstream FRAinput;
                string FRAlinef;
                FRAinput.open("friendA.txt");
                while (getline(FRAinput, FRAlinef)){
                    cout << FRAlinef << endl;
                }
                FRAinput.close();
                cout << '\n';
                
                //reveal on map
                gameMap.revealNpc(gameMap.getPlayerRowPosition(), gameMap.getPlayerColPosition());
                // check to see if interacted with alien yet
                if (npc.getComms() == false){
                    cout << "You have run into a friendly alien!\n";
                    
                    // check t device to start interaction
                    if (user.checkTranDevice() == true){
                        npc.setComms(true);
                        
                        //user chooses to ask alien for habitability or play rpc
                        char playerChoice;
                        cout << "You can communicate with this alien by either:\na. defeating them in rock, paper, scissors three times and get $200\nb. ask them about the habitability of this planet. The chance of the alien telling the truth is 70%.\nYou can only pick one option.\n";
                        cin >> playerChoice;
                        //Input Validation
                        while (playerChoice != 'a' && playerChoice != 'b'){
                            cout << "Invalid Input. Choose (a) or (b)\n";
                            cin >> playerChoice;
                        }
                        // interacion options execute w alien

                        //rock paper scisors
                        if (playerChoice == 'a'){
                            char choice;
                            //set wins for both to 0
                            int alienWins = 0;
                            int userWins = 0;
                            
                            //condition to stop rps is first to win 3 times
                            while(alienWins != 3 && userWins != 3){
                                //user picks rock paper or scissors
                                cout << "Choose:\nrock(r)\npaper(p)\nscissors(s)\n";
                                cin >> choice;
                                //make sure choice was r p or s
                                while(choice != 'r' && choice != 'p' && choice != 's'){
                                    cout<< "Invalid choice.\nChoose: rock(r), paper(p) or scissors(s)\n";
                                    cin >> choice;
                                }
                                
                                //1 = rock   2 = paper  3 = scissors
                                int userChoice;
                                if (choice == 'r'){
                                    userChoice = 1;
                                }
                                if (choice == 'p'){
                                    userChoice = 2;
                                }
                                if (choice == 's'){
                                    userChoice = 3;
                                }
                                
                                //alien asigned random rps
                                int alienChoice = rand() % 3 + 1;
                                string alienChoiceName;
                                if (alienChoice == 1){
                                    alienChoiceName = "rock";
                                }
                                if (alienChoice == 2){
                                    alienChoiceName = "paper";
                                }
                                if (alienChoice == 3){
                                    alienChoiceName = "scissors";
                                }
                                cout << "The Alien chose " << alienChoiceName << endl;
                                // rock(1) defeats scissors(3)
                                // scissors(3) defeats paper(2)
                                // paper(2) defeats rock(1)

                                /**
                                 * print who wins round
                                 * increment up variable for who wins the round
                                **/

                                //options for alien win
                                if( (alienChoice == 1 && userChoice == 3) || (alienChoice == 3 && userChoice == 2) || (alienChoice == 2 && userChoice == 1) ){
                                    alienWins++;
                                    cout<<"You lose this round"<<endl;  
                                }

                                //when alien doesn't win user wins
                                else{
                                    userWins++;
                                    cout<<"You win this round"<<endl;
                                }
                            }

                            //Print message to user when either user or alien wins 3 rounds of rps
                            if(alienWins == 3){
                                cout << "You failed to deafeat the alien! You don't get any money or intel." << endl;
                            }
                            else{
                                cout << "Congratulations! You defeated the alien and won $200\n";
                                user.setChangeAmountMoney(200);
                            }
                        }

                        //habitable or not
                        else if (playerChoice == 'b'){
                            //pick 1-10 for if alien will tell the truth 
                            //1-3 lie
                            //4-10 tell truth
                            int truthPercent = rand() % 10 + 1;
                            bool habitable; //Alien output habitability
                            //check if any crewmates are Chuck(increase truth by 10%)
                            if (user.getSelectCrewNames(0) == "Chuck" || user.getSelectCrewNames(1) == "Chuck"){
                                cout << "the alien is afraid of your crewmate Chuck. Aliens are 10% more likely to tell you the truth when Check is near by.\n";
                                // TP > 2 = alien tells true habitability
                                if (truthPercent > 2){
                                    //80% truth
                                    if(gameMap.isHabitable() == true){
                                        cout << "Hello human! I have looked around this planet and think this would make a great home for you.\n";
                                        habitable = true;
                                    }
                                    else{
                                        cout<<"Hello human! I have looked around this planet and think this would be a terrible choice for a home. Your fragile species could not handle these conditions, I would suggest finding someplace else."<<endl;
                                        habitable = false;
                                    }
                                }
                                //alien lies
                                else{
                                    if(gameMap.isHabitable() == false){
                                        cout << "Hello human! I have looked around this planet and think this would make a great home for you.\n";
                                        habitable = true;
                                    }
                                    else{
                                        cout<<"Hello human! I have looked around this planet and think this would be a terrible choice for a home. Your fragile species could not handle these conditions, I would suggest finding someplace else."<<endl;
                                        habitable = false;
                                    }
                                }
                            }
                            //No crewmates are in arr are Chuck
                            else{
                                //tell truth when TP is > 3
                                if (truthPercent > 3){
                                    //70% of alien truth
                                    if(gameMap.isHabitable() == true){
                                        cout << "Hello human! I have looked around this planet and think this would make a great home for you.\n";
                                        habitable = true;
                                    }
                                    else{
                                        cout<<"Hello human! I have looked around this planet and think this would be a terrible choice for a home. Your fragile species could not handle these conditions, I would suggest finding someplace else."<<endl;
                                        habitable = false;
                                    }
                                }
                                //alien lies
                                else{
                                    if(gameMap.isHabitable() == false){
                                        cout << "Hello human! I have looked around this planet and think this would make a great home for you.\n";
                                        habitable = true;
                                    }
                                    else{
                                        cout<<"Hello human! I have looked around this planet and think this would be a terrible choice for a home. Your fragile species could not handle these conditions, I would suggest finding someplace else."<<endl;
                                        habitable = false;
                                    }
                                }
                            }
                            //set npc info on log book based on what the alien reported
                            nPage.setNPC_a(habitable);

                            //alien says planet is habitable for humans
                            if (habitable == true){

                                //getline responses for when habitable
                                ifstream input;
                                string line;
                                input.open("alien_conversation.txt");
                                int count = 1; //lines printed option head

                                //ask which reponse
                                cout << "How would you like to respond?" << endl;
                                while(getline(input, line)){
                                    string rrHolder[2];
                                    split(line, ',', rrHolder, 2);

                                    //if line starts with h
                                    if (rrHolder[0] == "h"){
                                        cout << count << ". " << rrHolder[1] << endl;
                                        count++;
                                    }
                                }
                                int response;
                                cin >> response;

                                //make sure user inputs valid number for which response
                                while(response < 0 || response > count){
                                    //user asked to re enter
                                    cout << "Invalid input. Re-enter input numbers 1-" << count << endl;
                                    cin >> response;
                                }
                                input.close();
                            }

                            //alien reports planet as non habitable
                            else{
                                //getline responses
                                ifstream input;
                                string line;
                                input.open("alien_conversation.txt");
                                int count = 1;
                                cout << "How would you like to respond?" << endl;
                                while(getline(input, line)){
                                    string rrHolder[2];
                                    split(line, ',', rrHolder, 2);
                                    //prints only responses that start with n
                                    if (rrHolder[0] == "n"){
                                        cout << count << ". " << rrHolder[1] << endl;
                                        count++;
                                    }
                                }
                                input.close();
                                int response;
                                cin >> response;
                                
                                //input validation
                                while(response < 0 || response > count){
                                    //user enters input till valid input found
                                    cout<<"Invalid input. Re-enter input numbers 1-" << count << endl;
                                    cin >> response;
                                }
                                input.close();
                            }
                        }
                    }

                    //user did not have translator device
                    else{
                        //tell user that there is no translator
                        //tell user how to aquire
                        cout << "You do not have a translator and cannot communicate with this alien. This alien may have useful information for you and money to offer. You can purchase a translator from the store for $5000 if you would like to talk to the Alien. (must go to resource center, purchase a device, move positions, and then move back to current position)\n";
                    }

                }
                
                //already interacted
                else{
                    //no interaction more than once or user can do enough times and get enough data to know if alien is lying or not
                    cout << "You have already interacted with the NPC Alien.\n";
                }

                cout << '\n';
                cout << "\033[0mHit ENTER to continue."<< endl;
                cin.ignore();
                while(true){
                    if (cin.get() == '\n'){
                        break;
                    }
                }
            }


            /*---------if moved to Misfortune---------*/
            if (gameMap.isMisfortuneLocaton() == true){
                //show on map
                gameMap.revealMisfortune(gameMap.getPlayerRowPosition(), gameMap.getPlayerColPosition());
                
                //get which kind of misf
                int misFoAt = gameMap.getMisfortuneType();

                cout << "\033[31m";

                // make alien and battle alien
                if (misFoAt == 1){
                    // make new enwmy alien objct
                    HAlien enemy = HAlien();

                    //set randomly picked variables for new halien
                    enemy.setHAlienName();
                    enemy.setRandLVLalien();
                    enemy.setRandLVLplayer();

                    //variable for picking forfeit or battle   &    vect to hold which weapos user has
                    int battleChoice;
                    vector<int> weaponVect;
                    
                    //print Host A txt file
                    cout << '\n';
                    ifstream HAinput;
                    string HAlinef;
                    HAinput.open("hostA.txt");
                    while (getline(HAinput, HAlinef)){
                        cout << HAlinef << endl;
                    }
                    HAinput.close();
                    cout << '\n';

                    //inform user that they ran into an enemy alien
                    cout << "You just ran into "<< enemy.getHAlienName() << "! Think you can beat this dangerous alien for helping save humanity?\n";
                    cout << "1. Attack\n" << "2. Forfeit\n";
                    cin >> battleChoice;
                    // input validation
                    while (battleChoice != 1 && battleChoice != 2){
                        //tell user bad input then ask for new input (make sure user knows the parameters for input)
                        cout << "Invalid input. Enter either 1 or 2.\n";
                        cout << "1. Attack\n" << "2. Forfeit\n";
                        cin >> battleChoice;
                    }

                    //forfeit
                    if (battleChoice == 2){
                        //add damage points for each weapon to vect if user has the weapon
                        if (user.getNumLSaber() > 0){
                            weaponVect.push_back(1);
                        }
                        if (user.getNumBlaster() > 0){
                            weaponVect.push_back(2);
                        }
                        if (user.getNumBeamGun() > 0){
                            weaponVect.push_back(3);
                        }

                        //variable for picking random weapon in the vector
                        int weaponNum;


                        // pick rand weapon in inventory to take from user if weapon vect has 2 weapons
                        if (weaponVect.size() == 2){   
                            //rand weapon in weapon vect
                            weaponNum = rand() % 2 + 1;

                            //weapon name to print to user
                            string typeWeapon;

                            //use weaponNum as index in vect
                            if (weaponVect[weaponNum-1] == 1){
                                typeWeapon = "Light Saber";
                                user.setNumLSaber(-1);
                            }
                            if (weaponVect[weaponNum-1] == 2){
                                typeWeapon = "Blaster";
                                user.setNumBlaster(-1);
                            }
                            if (weaponVect[weaponNum-1] == 3){
                                typeWeapon = "Beam Gun";
                                user.setNumBeamGun(-1);
                            }

                            //tell user which weapon they lost by forfeiting 
                            cout << "You forfieted to the Alien. You lost a " <<  typeWeapon << " by forfieting." << endl;
                        }

                        // user only has 1 weapon
                        else if (weaponVect.size() == 1){
                            //weapon name to print to user
                            string typeWeapon;

                            //take type weapon from user if the weapon in the vect is certain type
                            if (weaponVect[0] == 1){
                                typeWeapon = "Light Saber";
                                user.setNumLSaber(-1);
                            }
                            if (weaponVect[0] == 2){
                                typeWeapon = "Blaster";
                                user.setNumBlaster(-1);
                            }
                            if (weaponVect[0] == 3){
                                typeWeapon = "Beam Gun";
                                user.setNumBeamGun(-1);
                            }
                            //tell user which weapon type they lost by forfeiting 
                            cout << "You forfieted to the Alien. You lost a " <<  typeWeapon << " by forfieting." << endl;
                        }

                        // user has no weapons
                        else if (weaponVect.size() == 0){
                            //no weapon so user loses cost of most expensive weapon
                            cout << "You forfieted to the Alien and lost $5000." << endl;
                            user.setChangeAmountMoney(-5000);
                        }
                    }
                    
                    //fight the enemy alien
                    if (battleChoice == 1){
                        // add weaposns held by user into vect
                        if (user.getNumLSaber() > 0){
                            weaponVect.push_back(1);
                        }
                        if (user.getNumBlaster() > 0){
                            weaponVect.push_back(2);
                        }
                        if (user.getNumBeamGun() > 0){
                            weaponVect.push_back(3);
                        }

                        int pickWeapon;         //weapon user wants to use in battle
                        int weaponDamage;       //damage weapon does(stored in vect)
                        
                        //weapon name to print to user
                        string typeWeapon1;
                        string typeWeapon2;

                        //user only has one weapon
                        if (weaponVect.size() == 1){
                            //get weapon type to print
                            if (weaponVect[0] == 1){
                                typeWeapon1 = "Light Saber";
                            }
                            if (weaponVect[0] == 2){
                                typeWeapon1 = "Blaster";
                            }
                            if (weaponVect[0] == 3){
                                typeWeapon1 = "Beam Gun";
                            }

                            //prompt user to pick weapon and begin battle
                            cout << "Let's attack! Choose a weapon from your arsenal quickly and attack before the enemy advances!" << endl;
                            cout << "1. " << typeWeapon1 << endl;
                            cin >> pickWeapon;
                            //check for valid input
                            while (pickWeapon != 1){
                                //tell user that their input is invalid
                                cout << "Invalid input.\n";
                                cout << "Let's attack! Choose a weapon from your arsenal quickly and attack before the enemy advances!" << endl;
                                cout << "1. " << typeWeapon1 << endl;
                                cin >> pickWeapon;
                            }

                            //set weapon damage for weapon picked
                            if (pickWeapon == 1){
                                weaponDamage = weaponVect[0];
                            }
                        }

                        //user has two weapons
                        if (weaponVect.size() == 2){
                            //asign name to first option printed
                            if (weaponVect[0] == 1){
                                typeWeapon1 = "Light Saber";
                            }
                            if (weaponVect[0] == 2){
                                typeWeapon1 = "Blaster";
                            }
                            if (weaponVect[0] == 3){
                                typeWeapon1 = "Beam Gun";
                            }

                            //asign name to second option printed
                            if (weaponVect[1] == 1){
                                typeWeapon2 = "Light Saber";
                            }
                            if (weaponVect[1] == 2){
                                typeWeapon2 = "Blaster";
                            }
                            if (weaponVect[1] == 3){
                                typeWeapon2 = "Beam Gun";
                            }

                            //prompt user to pick weapon and begin battle
                            cout << "Let's attack! Choose a weapon from your arsenal quickly and attack before the enemy advances!" << endl;
                            cout << "1. " << typeWeapon1 << "\n2. " << typeWeapon2 << endl;
                            cin >> pickWeapon;
                            //check for valid input
                            while (pickWeapon != 1 && pickWeapon != 2){
                                //tell user that their input is invalid
                                cout << "Invalid input.\n";
                                cout << "Let's attack! Choose a weapon from your arsenal quickly and attack before the enemy advances!" << endl;
                                cout << "1. " << typeWeapon1 << endl;
                                cout << "2. " << typeWeapon2 << endl;
                                cin >> pickWeapon;
                            }
                            
                            // set damage aligned with user input
                            if (pickWeapon == 1){
                                weaponDamage = weaponVect[0];
                            }
                            if (pickWeapon == 2){
                                weaponDamage = weaponVect[1];
                            }
                        }

                        //check if one of crewmateas is Nereys to get advantage in battle
                        int countFighters = 0;
                        for (int j = 0; j < 3; j++){
                            if (user.getSelectCrewNames(j) == "Nerys"){
                                cout << "Your odds in this fight are looking 10 percent better. Your crewmate Nerys is a world class fighter.\n";
                                countFighters++;
                            }
                        }

                        // asign variables for planet # and the rand LVL for player and alien
                        int aLVL = enemy.getRandLVL_A();
                        int uLVL = enemy.getRandLVL_U();
                        int pNUM = user.getPlanetsExpl();

                        //battle equation to calculate winloss number
                        int battleWinLoss = ((uLVL * weaponDamage)*(1 + countFighters)) - (aLVL * pNUM);
                        
                        //user wins
                        if (battleWinLoss > 0){
                            //print UKHA asci art
                            cout << '\n';
                            string hostK_line;
                            ifstream hostK_input;
                            hostK_input.open("hostA_killedByU.txt");
                            while(getline(hostK_input, hostK_line)){
                                cout << hostK_line << endl;
                            }
                            hostK_input.close();
                            cout << '\n';

                            // check if user has max weapons in order to get reward of money or new weapon
                            if (user.getNumBlaster() + user.getNumBeamGun() + user.getNumLSaber() < 2){
                                //random weapon won pick 1-3
                                int weaponTypeAdd = rand() % 3 + 1;

                                //depends on weaponTypeAdd and then add the weapon to user inventory
                                if (weaponTypeAdd == 1){
                                    user.setNumLSaber(1);
                                    cout << "You defeated the alien and won a light saber. During the battle your space suit lost 10 pecent of its health.\n";
                                }
                                if (weaponTypeAdd == 2){
                                    user.setNumBlaster(1);
                                    cout << "You defeated the alien and won a blaster. During the battle your space suit lost 10 pecent of its health.\n";
                                }
                                if (weaponTypeAdd == 3){
                                    user.setNumBeamGun(1);
                                    cout << "You defeated the alien and won a Beam Gun. During the battle your space suit lost 10 pecent of its health.\n";
                                }
                            }

                            //user wins and has max weapons
                            else{
                                //give user amount money for most expensive weapon
                                cout << "You defeated the alien and earned $5000. During the battle your spacesuit lost 10 percent of its health.\n";
                                user.setChangeAmountMoney(5000);
                            }

                            //change suit health
                            user.setSuitHealth(-0.1);
                        }

                        // User loses battle
                        else{
                            //print HAKU asci art
                            cout << '\n';
                            string hostK_line;
                            ifstream hostK_input;
                            hostK_input.open("hostA_killedU.txt");
                            while(getline(hostK_input, hostK_line)){
                                cout << hostK_line << endl;
                            }
                            hostK_input.close();
                            cout << '\n';

                            //change in suit health
                            int switchInt = user.getSpaceSuitLevel();
                            double loseH = (100.00 - (20.00 * double(switchInt) ) );

                            //User loses weapon used in battle
                            if (pickWeapon == 1){
                                //tell user they lost and which weapon was lost & type
                                cout << "You were defeated by the Alien and lost your " << typeWeapon1 << " in battle. Your health decreased 30 percent and your space suit health decreased by " << loseH << "%." << endl;
                                //get trype from vect at position 0
                                if (weaponVect[0] == 1){
                                    user.setNumLSaber(-1);
                                }
                                if (weaponVect[0] == 2){
                                    user.setNumBlaster(-1);
                                }
                                if (weaponVect[0] == 3){
                                    user.setNumBeamGun(-1);
                                }
                            }
                            if (pickWeapon == 2){
                                //tell user they lost and which weapon was lost & type
                                cout << "You were defeated by the Alien and lost your " << typeWeapon2 << " in battle. Your health decreased 30 percent and your space suit health decreased by " << loseH << "%." << endl;
                                //get trype from vect at position 0
                                if (weaponVect[1] == 1){
                                    user.setNumLSaber(-1);
                                }
                                if (weaponVect[1] == 2){
                                    user.setNumBlaster(-1);
                                }
                                if (weaponVect[1] == 3){
                                    user.setNumBeamGun(-1);
                                }
                            }

                            //change player health
                            user.setPlayerHealth(-0.30);

                            //enter as part of 1 not 100
                            loseH = loseH * 0.01;

                            //change suit health
                            user.setSuitHealth(-1.00 * loseH);

                            //check for user crewmate Lola
                            if(user.getSelectCrewNames(0) == "Lola" || user.getSelectCrewNames(1) == "Lola"){
                                cout << "lucky you, your crewmate, Lola is great at healing battle wounds. She gave you 5 percent health back.\n";
                                user.setPlayerHealth(0.05);
                            }
                        }        
                    }
                    
                    cout << "\033[0mHit ENTER to continue."<< endl;
                    cin.ignore();
                    while(true){
                        if (cin.get() == '\n'){
                            break;
                        }
                    }
                }

                // weather
                if (misFoAt == 2){
                    ifstream wInput;
                    string wLine;
                    wInput.open("weather.txt");
                    while(getline(wInput, wLine)){
                        cout << wLine << endl;
                    }
                    wInput.close();

                    //calculate health and space suit health then print to user misforune and losses in health
                    double i = (double)user.getSpaceSuitLevel();
                    double suitEQ = (i * 0.1) - 0.5;
                    cout << "\nUnlucky, it appears that you got stuck in some unpredictable weather. High winds and flying debree deducted 10 percent of your health and " << -suitEQ*100 << " percent of your suit's health.\n";

                    //change health for space suit and player
                    user.setSuitHealth(suitEQ);
                    user.setPlayerHealth(-0.1);

                    cout << "\033[0m\nHit ENTER to continue." << endl;
                    cin.ignore(2, '\n');
                    while(true){
                        if (cin.get() == '\n'){
                            break;
                        }
                    }
                }

                // fall in crater
                if (misFoAt == 3){
                    ifstream FInput;
                    string FLine;
                    FInput.open("falling.txt");
                    while(getline(FInput, FLine)){
                        cout << FLine << endl;
                    }
                    FInput.close();
                    double i = (double)user.getSpaceSuitLevel();
                    double suitEQ = (i * 0.1) - 0.5;
                    cout << "\nUnlucky, it appears that you fell into a crater. Impact from your big fall deducted 50 percent of your health and " << -suitEQ*100 << " percent of your suit's health.\n";
                    user.setPlayerHealth(-0.5);
                    user.setSuitHealth(suitEQ);
                    cout << "\033[0m\nHit ENTER to continue." << endl;
                    cin.ignore(2, '\n');
                    while(true){
                        if (cin.get() == '\n'){
                            break;
                        }
                    }
                }

                // space sick
                if (misFoAt == 4){
                    ifstream SInput;
                    string SLine;
                    SInput.open("sick.txt");
                    while(getline(SInput, SLine)){
                        cout << SLine << endl;
                    }
                    SInput.close();
                    cout << "\nYour feeling ill from space travel and constant gravity changes. Lack of a health care plan results in a loss of 30 percent health.\n";
                    user.setPlayerHealth(-0.3);
                    cout << "\033[0m\nHit ENTER to continue." << endl;
                    cin.ignore(2, '\n');
                    while(true){
                        if (cin.get() == '\n'){
                            break;
                        }
                    }
                }
            }


            /*------------if moved to site------------*/
            if (gameMap.isSiteLocation() == true){
                //print site txt file
                cout << "\033[34m" << '\n';
                ifstream SITEinput;
                string SITElinef;
                SITEinput.open("site.txt");
                while (getline(SITEinput, SITElinef)){
                    cout << SITElinef << endl;
                }
                SITEinput.close();
                cout << '\n';
                
                //reveal on map
                gameMap.revealSite(gameMap.getPlayerRowPosition(), gameMap.getPlayerColPosition());

                // test if user has been to site already
                bool testVisit = false;
                for (int i = 0; i < 7; i++){
                    if (site_VisitAgain[i][0] == gameMap.getPlayerColPosition() && site_VisitAgain[i][1] == gameMap.getPlayerRowPosition()){
                        testVisit = true;
                    }
                }


                // if first visit run do site activity
                if (testVisit == false){
                    //set site visit for current site # at 
                    site_VisitAgain[indexSite][0] = gameMap.getPlayerColPosition(); 
                    site_VisitAgain[indexSite][1] = gameMap.getPlayerRowPosition();
                    indexSite++;
                    
                    //tell user they found a site
                    cout << "Good job, you have found a site that may have some useful information about this planet. If you get this question right, the information will be revealed.\nIf you answer incorrectly though, you will become more frustrated and lose 1 percent health from stress.\n";
                    
                    // 4 answrs then question in arr
                    string valuesToPrint[5];
                    askRand(valuesToPrint); //function to refer to outside of int main
                    
                    int idx = rand() % 3;
                    char arrLetr[4] = {'a', 'b', 'c', 'd'};
                    string traitArr[7] = {"0", "water", "oxygen", "fertile soil", "bad atmosphere", "toxic gas", "bad temperature"};
                    
                    cout << valuesToPrint[4] << endl;

                    // print corect answr choice in rand position
                    int index1 = 1;

                    //print options with choice letter
                    for (int i = 0; i < 4; i++){
                        if (i == idx){
                            cout << arrLetr[i] << ". " << valuesToPrint[0] << endl;
                        }
                        else{
                            cout << arrLetr[i] << ". " << valuesToPrint[index1] << endl;
                            index1++;
                        }
                    }
                    
                    //get user input
                    char userIn;
                    cin >> userIn;

                    //test if mary is a crewmat
                    bool sciMary = true;
                    //incorect answr
                    while (userIn != arrLetr[idx]){
                        if ((user.getSelectCrewNames(0) == "Mary" || user.getSelectCrewNames(1) == "Mary") && sciMary == true){
                            sciMary = false;
                            cout << "Incorrect answer! Your crewmate Mary bailed you out this time. Try again.\n";
                            cin >> userIn;
                        }
                        else{
                            cout << "\033[31mIncorrect answer! Your health was reduced by 1% because of frustration. Try again.\n\033[34m";
                            user.setPlayerHealth(-0.01);
                            cin >> userIn;
                        }
                    }

                    // corect answr give user the site trait
                    if (userIn == arrLetr[idx]){
                        cout << "Correct answer!\n";
                        int whichTrait = gameMap.getSiteTrait();
                        cout << "You just discovered that this planet has " << traitArr[whichTrait] << ". This discovery has been added to your log book.\n";
                        
                        // add whichTrait to LB acording to H or NH trait
                        if (whichTrait == 1 || whichTrait == 2 || whichTrait == 3){
                            nPage.setH_traits(traitArr[whichTrait]);
                        }
                        if (whichTrait == 4 || whichTrait == 5 || whichTrait == 6){
                            nPage.setNH_traits(traitArr[whichTrait]);
                        }
                    }
                }
                
                // visited already consider position as free space
                else{
                    cout << "\nYou have already visited this site and discovered one of this planet's traits from this site.\n";
                }

                cout << "\033[0mHit ENTER to continue."<< endl;
                cin.ignore();
                while(true){
                    if (cin.get() == '\n'){
                        break;
                    }
                }
            }
        }


        /****VIEW STATUS****/
        else if (userChoiceMM == 2){
            cout << '\n';
            ifstream Sinput;
            string Slinef;
            Sinput.open("status.txt");
            while (getline(Sinput, Slinef)){
                cout << Slinef << endl;
            }
            Sinput.close();
            cout << '\n';

            //percent bars for health, suit health, and fuel
            cout << "Health:\n";
            double x = user.getPlayerH() * 100;
            int playerH_percent = int (x);
            percentageBar(playerH_percent);

            cout << "Space Suit Health:\n";
            double y = user.getSuitH() * 100;
            int suitH_percent = int (y);
            percentageBar(suitH_percent);
            
            cout << "Fuel tank:\n";
            double z = (double(user.getAmountFuel()) / 300000.0) * 100.0;
            int fTank_percent = int (z);
            percentageBar(fTank_percent);


            // p explored, reports acurrate or not(int value)
            cout << "\nPlanets explored: " << user.getPlanetsExpl() << endl;
            cout << "Planets accurately reported habitable: " << user.getNumAccurateReport() << endl;
            cout << "Planets inaccurately reported habitable: " << user.getNumInaccurateReport() << endl;


            // humans sent to planet either dead or alive
            cout << "\nHumans saved (in millions): " <<  aliveH << endl;
            cout << "Humans perished (in millions): " << deadH << endl;


            // check if user has translator
            int TD_TF = 0;
            if (user.checkTranDevice() == true){
                TD_TF += 1;
            }


            // list of all items in user inventory
            cout << "\nMoney - $" << user.getAmountMoney() << "\nFuel - " << user.getAmountFuel() << " gallons\n" << "Translator - " << TD_TF << "\nLightsabers - " << user.getNumLSaber() << "\nBlasters - " << user.getNumBlaster() << "\nBeam Guns - " << user.getNumBeamGun() << "\nSpace Suit Grade - " << user.getSpaceSuitLevel() << "\nMedical Kits - " << user.getNumMedKit() << endl;
            cout << endl;
        }


        /****VIEW LOGBOOK****/
        else if (userChoiceMM == 3){
            cout << '\n' << user.getPlayerName() << "'s Log Book page #" << user.getPlanetsExpl() << ": Planet " << nPage.getP_Name() << endl;
            cout << "Planet size (in diameter of 1000 kms): " << nPage.getP_Diameter() << endl;
            cout << "Planet carrying capacity (in millions): " << nPage.getCarry_Cap() << endl;
            cout << "Habitable traits discovered: " << nPage.getH_traits() << endl;
            cout << "Non-habitable traits discovered: " << nPage.getNH_traits() << endl;
            cout << "Friendly alien assessment: " << nPage.getNPC_a() << endl;
                    // Mary’s Log Book page #5: Planet XY78V2-Eura
                    // Planet size (in diameter of 1000 kms): 6
                    // Planet’s carrying capacity (in millions): 3692
                    // Habitable traits discovered: Water
                    // Non-habitable traits discovered: Toxic gas, Bad atmosphere
                    // Friendly alien assessment: Non-habitable
        }
    

        /****RESOURCE CENTER****/
        else if (userChoiceMM == 4){
            checkIfleave = false;
            while(checkIfleave == false){
                vector<string> choiceVect;  //vector to hold menu options that are going to print
                int pChoice;    //variable for user menu choice
                int index = 1;  //index number of options printed to menu/numbering of menu left side
                cout << "Which item do you wish to buy?\n";
                // each menu line is printed as:  "index. menuItemOption"

                //num weapons < 2 to be able to print on menu or buy
                if ((user.getNumLSaber() + user.getNumBlaster() + user.getNumBeamGun()) < 2){
                    cout << index << ". Weapon\n";  
                    index++;
                    choiceVect.push_back("weapon");
                }
                //user must not already own Translator for user to be able to print it on menu or buy 
                if (user.checkTranDevice() == false){
                    cout << index << ". Translator\n";
                    index++;
                    choiceVect.push_back("translator");
                }
                //make sure suit level is less than 5 to be able to print on menu or buy
                if (user.getSpaceSuitLevel() < 5){
                    cout << index << ". Space suit\n";
                    index++;
                    choiceVect.push_back("spacesuit");
                }
                //check & make sure user has less than 5 medkit to be able to print on menu or buy
                if (user.getNumMedKit() < 5){
                    cout << index << ". Medical Kits\n";
                    index++;
                    choiceVect.push_back("medkits");
                }
                //only print option if user has less than 300000 gal fuel
                if (user.getAmountFuel() < 300000){
                    cout << index << ". Fuel\n";
                    index++;
                    choiceVect.push_back("fuel");
                }
                //Leave option always on menu
                cout << index << ". Leave" << endl;
                choiceVect.push_back("leave");
                cin >> pChoice;
                
                // input validation (make sure user picked an option on menu)
                while (pChoice <= 0 || pChoice > index){
                    //RePrint menu options and tell user Invalid input
                    cout << "Invalid input.\nWhich item do you wish to buy?\n";
                    index = 1;
                    if ((user.getNumLSaber() + user.getNumBlaster() + user.getNumBeamGun()) < 2){
                        cout << index << ". Weapon\n";
                        index++;
                    }
                    if (user.checkTranDevice() == false){
                        cout << index << ". Translator\n";
                        index++;
                    }
                    if (user.getSpaceSuitLevel() < 5){
                        cout << index << ". Space suit\n";
                        index++;
                    }
                    if (user.getNumMedKit() < 5){
                        cout << index << ". Medical Kits\n";
                        index++;
                    }
                    if (user.getAmountFuel() < 300000){
                        cout << index << ". Fuel\n";
                        index++;
                    }
                    cout << index << ". Leave\n";
                    //user input again than check again with while loop
                    cin >> pChoice;
                }

                string selectedFromMenu = choiceVect[pChoice-1];    //menu option strng is gotten from choice vector and compared to options bellow
                int dolAmountC = 0;                                 //holds amount money before purchase in sub menus
                int curAmount = user.getAmountMoney();              //amount money user has before purchase

             // WEAPON BUY MENU
                if (selectedFromMenu == "weapon"){
                    int whichWeapon;    //pick from weapons menu
                    int numWeapon;      //number of selected weapon from weapons menu
                    bool quit = true;   //leave weapon menu using while true and change if leave
                    while (quit){
                        int totalWeapons = user.getNumLSaber() + user.getNumBlaster() + user.getNumBeamGun();       //after every pick reset keep updated total weapons count

                        //gives user options for weapons to buy
                        int whichWeapon;
                        cout << "Which weapon type do you wish to buy?\n1. Light Saber\n2. Blaster\n3. Beam Gun\n4. Go back to menu\n";
                        cin >> whichWeapon;
                        //check valid input
                        while(whichWeapon > 4 || whichWeapon < 0){
                            cout << "Re-enter a valid option 1 through 4\n";
                            cin >> whichWeapon;
                        }

                        //give price for each weapon if selected and ask how many of each weapon the user would like to buy
                        //input validation of check total weapons 

                        //buy lightt saber
                        if (whichWeapon == 1){
                            cout << "How many Light Sabers do you want ($1000 each)?\n";
                            cin >> numWeapon;
                            // input validation
                            while (totalWeapons + numWeapon > 2 || numWeapon < 0){
                                cout << "You cannot enter negative numbers or have more than two weapons at the same time, re-enter a valid quantity.\n";
                                cin >> numWeapon;
                            }
                            // if enough money
                            if (curAmount - dolAmountC >= 1000*numWeapon){
                                user.setNumLSaber(numWeapon);                   //change number of the selected weapon to user input amount
                                dolAmountC += 1000*numWeapon;                   //calc $ after purchase
                                int answ = curAmount - dolAmountC;              //calc $ after purchase
                                cout << "You have $" << answ << " left.\n";
                            }
                            //not enough $
                            else{
                                cout << "You do not have enough money for that.\n";
                            }
                        }
                        //buy Blaster
                        if (whichWeapon == 2){
                            cout << "How many Blasters do you want ($2000 each)?\n";
                            cin >> numWeapon;
                            //input validation.
                            while (totalWeapons + numWeapon > 2 || numWeapon < 0){
                                cout << "You cannot enter negative numbers or have more than two weapons at the same time, re-enter a valid quantity.\n";
                                cin >> numWeapon;
                            }
                            //user has suficient funds
                            if (curAmount - dolAmountC >= 2000*numWeapon){
                                user.setNumBlaster(numWeapon);             //change number of the selected weapon to user input amount
                                dolAmountC += 2000*numWeapon;              //calc $ after purchase
                                int answ = curAmount - dolAmountC;         // ^^
                                cout << "You have $" << answ << " left.\n";
                            }
                            //not enough $
                            else{
                                cout << "You do not have enough money for that.\n";
                            }
                        }
                        //buy Beam Gun
                        if (whichWeapon == 3){
                            cout << "How many Beam Guns do you want ($5000 each)?\n";
                            cin >> numWeapon;
                            while (totalWeapons + numWeapon > 2 || numWeapon < 0){
                                cout << "You cannot enter negative numbers or have more than two weapons at the same time, re-enter a valid quantity.\n";
                                cin >> numWeapon;
                            }
                            //calculate cost if check enough $ is enough
                            if (curAmount - dolAmountC >= 5000*numWeapon){
                                user.setNumBeamGun(numWeapon);             //change number of the selected weapon to user input amount
                                dolAmountC += 5000*numWeapon;              //calc $ after purchase
                                int answ = curAmount - dolAmountC;         // ^^
                                cout << "You have $" << answ << " left.\n";
                            }
                            //not enough $
                            else{
                                cout << "You do not have enough money for that.\n";
                            }
                        }

                        //back to main menu
                        if (whichWeapon == 4){
                            user.setChangeAmountMoney(-dolAmountC);     //change amount of money with variable for total spent
                            cout << "You now have $" << user.getAmountMoney() << " remaining.\n";   //tell user amount left
                            curAmount = user.getAmountMoney();  //re-set to current money with getter
                            quit = false;   //exit loop condition changed
                        }
                    }
                }
                
             // TRANSLATOR DEVICE PURCHASE
                else if(selectedFromMenu == "translator"){
                    //prompt user to answer whether they want a translator or not
                    //inform user of cost
                    int yesNo;
                    cout << "Would you like to purchase a translator device ($5000)? 1 for yes, 0 for no.\n";
                    cin >> yesNo;
                    // input validation, make sure 1 or 0 else say invalid and reprompt
                    while (yesNo != 0 && yesNo != 1){
                        cout << "Re-enter a valid input: 1 for yes, 0 for no.\n";
                        cin >> yesNo;
                    }
                    //user wants to purchase
                    if (yesNo == 1){
                        //check to make sure user has enough money
                        if (user.getAmountMoney() >= 5000){             // user has money
                            user.setTDevice(true);                      // give user translator device
                            user.setChangeAmountMoney(-5000);           // deduct cost of device from current amount money that user has
                            //tell user their current balance
                            cout << "You have $" << user.getAmountMoney() << " left.\n";
                        }
                        //user does not have enough
                        else{    
                            //tell user that they are short money    
                            cout << "You do not have enough money for that.\n";
                            cout << "You have $" << user.getAmountMoney() << " left.\n";
                        }
                    }
                    //no longer wants to make purchase
                    else if (yesNo == 0){
                        //tell user that they are going back to big rc menu & money left
                        cout << "You have $" << user.getAmountMoney() << " left. You are now being redirect to the main Resource Center menu.\n";
                    }
                }
                
             // UPGRADE SUIT
                else if (selectedFromMenu == "spacesuit"){
                    int newGradeChoice;     //choice for upgrade that user 
                    vector<int> ssVect;     //suits that can be bought/upgraded to

                    //decide upgrade or not
                    cout << "Which would you like to upgrade your spacesuit to? The space suit health will be set to 100%.\n";
                    int index = 1; //for numbered list
                    for (int i = user.getSpaceSuitLevel()+1; i < 6; i++){
                        ssVect.push_back(i);        //put levels to buy into int vector
                        cout << index << ". Space suit grade " << i << " costs $" << 5000*index << endl;   //numOnList. Space suit grade i costs $(5000*index)       pay difference:price of suit is not constant:price/suitLVL is constant
                        index++; //add so that next number in list is not same
                    }
                    cin >> newGradeChoice;
                    //INPUT VALIDATION
                    while (newGradeChoice > index-1 || newGradeChoice <= 0){        //must stay in bounds of printed index
                        cout << "Invalid input. Make sure to enter a number between 1 and " << index-1 << endl;
                        cin >> newGradeChoice;
                    }
                    //check if user has enough money to make purchase
                    if (newGradeChoice*5000 <= user.getAmountMoney()){
                        int gradeLVL_Holder = ssVect[newGradeChoice-1]; //get level for menu choice made
                        user.setSuitLVL(gradeLVL_Holder);   //change grade of suit
                        user.resetSH();              //sets suit health to be full
                        user.setChangeAmountMoney(-(5000*newGradeChoice));          //deduct cost from user money balance

                        //confirm suit upgrade by telling user how much money they have left
                        cout << "Your spacesuit is now a grade " << newGradeChoice << '.' << endl;
                        cout << "You have $" << user.getAmountMoney() << " left.\n";
                    }
                    //user balance is too low
                    else{
                        cout << "You do not have enough money for that.\n";
                        cout << "You have $" << user.getAmountMoney() << " left.\n";
                    }
                }
                
             // PURCHASE MEDKITS
                else if (selectedFromMenu == "medkits"){
                    int numMK;      // # of medkits user wants to buy
                    //asks user for a number of MK, tells tham how may in inventory, and the max number they can have in their inventory
                    cout << "How many medical kits would you like to purchase ($2000 each)? You currently have " << user.getNumMedKit() << ", and at any time can at most have 5.\n";
                    cin >> numMK;
                    // check input validation
                    while (numMK+user.getNumMedKit() > 5 || numMK < 0){
                        //inform user of Invalid input and tell the user perameters
                        cout << "Invalid input. You cannot have more than 5 medkits and you cannot enter numbers less than 0. Re-enter a valid quantity.\n";
                        cin >> numMK;
                    }
                    // make sure user has enough money
                    if (numMK*2000 <= user.getAmountMoney()){
                        user.setNumMedKit(numMK);           //give user # of medkits requested
                        user.setChangeAmountMoney(-(2000*numMK));   //charge user balance
                        cout << "You have $" << user.getAmountMoney() << " left.\n";    //tell user current balance
                    }
                    //not enough money
                    else{
                        //tell user purchase canceled due to insuficient funds and telll them current ballance
                        cout << "You do not have enough money for that.\n";
                        cout << "You have $" << user.getAmountMoney() << " left.\n";
                    }
                }
                
             // PURCHASE FUEL
                else if (selectedFromMenu == "fuel"){
                    int ammountFuelInput;   //store user input
                    //tell user curent fuel in gal, max amount, price per unit galons, and input perameters
                    cout << "You have " << user.getAmountFuel() << " gallons of fuel. You may only purchase up to a total of 300k gallons of fuel (current fuel + amount to buy).\nHow many gallons would you like to purchase ($1000 per 10,000 gallons)? Input must be in multiples of 10,000.\n";
                    cin >> ammountFuelInput;
                    //input validation make
                    while (ammountFuelInput%10000!=0 || user.getAmountFuel()+ammountFuelInput > 300000){
                        //ask user to enter new quantity when input does not follow perameters
                        cout << "Input must be in multiples of 10,000 and cannot add up to exceed 300k gallons when added to your current amount. Re-enter a valid quantity.\n";
                        cin >> ammountFuelInput;
                    }
                    // make sure that user has enough money for selected amount fuel
                    if (ammountFuelInput/10 <= user.getAmountMoney()){
                        user.setChangeAmountFuel(ammountFuelInput);
                        user.setChangeAmountMoney(-(ammountFuelInput/10));
                        cout << "You have $" << user.getAmountMoney() << " left.\n";
                    }
                    //user balance too low
                    else{
                        //tell user that too little ballance and also tell user their current ballance
                        cout << "You do not have enough money for that.\n";
                        cout << "You have $" << user.getAmountMoney() << " left.\n";
                    }
                }

             // LEAVE MENU
                else if(selectedFromMenu == "leave"){
                    checkIfleave = true;                //set the looping bool to equal true so that while(bool = is no longer false)
                }
            }
            cout << "Great purchase, Elon is sending a Falcon rocket with your supplies to you right now.\n" << endl;
        }


        /****REPORT PLANET AS HABITABLE****/
        else if (userChoiceMM == 5){
            //get fuel cost with planet number
            int nP = user.getPlanetsExpl();
            int fCostNP = fuelcost_Recur(nP);
            int x = fCostNP/10;
            if (user.getSelectCrewNames(0) == "Sherry" || user.getSelectCrewNames(1) == "Sherry"){
                fCostNP -= x;
            }

            //not enough fuel to travel after reporting
            if (user.getAmountFuel() < fCostNP){
                //tell user not enough and how to get more fuel
                cout << "When reporting a planet, you will redirect to a new planet. You do not have enough fuel to make it to the next planet. Keep exploring to gain money, or go to the resource center and buy more fuel\n";
            }
            //user has enough fuel
            else {
                //show current log book page and ask if user is sure about report
                cout << "Are you sure you want to report back to mission control that this planet is habitable? As a reminder, here is your log book:\n" << endl;

                cout << user.getPlayerName() << "'s Log Book page #" << user.getPlanetsExpl() << ": Planet " << nPage.getP_Name() << endl;
                cout << "Planet size (in diameter of 1000 kms): " << nPage.getP_Diameter() << endl;
                cout << "Planet carrying capacity (in millions): " << nPage.getCarry_Cap() << endl;
                cout << "Habitable traits discovered: " << nPage.getH_traits() << endl;
                cout << "Non-habitable traits discovered: " << nPage.getNH_traits() << endl;
                cout << "Friendly alien assessment: " << nPage.getNPC_a() << endl;
                cout << "\nReporting this planet is irreversible. If the planet is in fact habitable, you will save " << nPage.getCarry_Cap() << " million people! If it's not, the people sent to this planet will die.\n" << endl;    
                
                cout << "Before Reporting, note: you can't go to a new planet unless you have suficient money or at least " << fCostNP << " gallons of fuel. Ready to report? [y/n]\n";
                
                //go through with reporing user answer
                char yesNo_response;
                cin >> yesNo_response;
                while (tolower(yesNo_response) != 'y' && tolower(yesNo_response) != 'n'){
                    cout << "Invalid input. Re-enter response as either (y) or (n).\n";
                    cin >> yesNo_response;
                }
                
                // do report when user want to report
                if (tolower(yesNo_response) == 'y'){
                    pReported = true;
                    //correct report
                    if (gameMap.isHabitable() == true){
                        //all humans off world
                        if (total_HumansToSave - stoi(nPage.getCarry_Cap()) <= 0){
                            aliveH += total_HumansToSave;
                            user.setAccurateRep(1);
                            cout << "All of humanity has left Earth! Your mission is over.\n";
                            ENDgameM = true;
                        }
                        //humans on earth still
                        else{
                            userChoiceMM = 6;
                        }
                    }

                    // incorect report
                    else{
                        //all humans off world
                        if (total_HumansToSave - stod(nPage.getCarry_Cap()) <= 0){
                            deadH += total_HumansToSave;
                            user.setInaccurateRep(1);
                            cout << "All of humanity has left Earth! Your mission is over.\n";
                            ENDgameM = true;
                        }

                        //humans on earth still
                        else{
                            userChoiceMM = 6;
                        }
                    }
                }
            }
        }
        

        /*****ENTER WORMHOLE TO NEXT PLANET*****/
        if (userChoiceMM == 6){
            int nP = user.getPlanetsExpl();
            int fCostNP = fuelcost_Recur(nP);
            int y = fCostNP/10;
            if (user.getSelectCrewNames(0) == "Sherry" || user.getSelectCrewNames(1) == "Sherry"){
                fCostNP -= y;
            }

            if(fCostNP <= user.getAmountFuel()){
                //user reported planet
                if(pReported == true){
                    //was the map habitable or not
                    if (gameMap.isHabitable() == true){

                        aliveH += stoi(nPage.getCarry_Cap());
                        user.setAccurateRep(1);
                        total_HumansToSave -= stoi(nPage.getCarry_Cap());
                        cout << "Congratulations! You have saved " << nPage.getCarry_Cap() << " million people! Planet " << nPage.getP_Name() << " is their new home. You have " << total_HumansToSave << " million people to save left.\n";
                    
                    }
                    else{

                        deadH += stoi(nPage.getCarry_Cap());
                        total_HumansToSave -= stoi(nPage.getCarry_Cap());
                        user.setInaccurateRep(1);
                        cout << "Your choice has led to tragedy. " << nPage.getCarry_Cap() << " million people were sent to planet " << nPage.getP_Name() << " and died due to its in-hospitable conditions. You still have " << total_HumansToSave << " million people left to save, so keep exploring.\n";
                    
                    }

                    //reset visited sites
                    for (int i = 0; i < 7; i++){
                        site_VisitAgain[i][0] = -1;
                        site_VisitAgain[i][1] = -1;
                    }

                    //temp string to make new txt file name
                    string TXT_file = "";
                    TXT_file += nPage.getP_Name();
                    TXT_file += ".txt";
                    
                    //make new txt file to store the logbook
                    ofstream outF;
                    outF.open(TXT_file);

                        outF << user.getPlayerName() << "'s Log Book page #" << user.getPlanetsExpl() << ": Planet " << nPage.getP_Name() << endl;
                        outF << "Planet size (in diameter of 1000 kms): " << nPage.getP_Diameter() << endl;
                        outF << "Planet carrying capacity (in millions): " << nPage.getCarry_Cap() << endl;
                        outF << "Habitable traits discovered: " << nPage.getH_traits() << endl;
                        outF << "Non-habitable traits discovered: " << nPage.getNH_traits() << endl;
                        outF << "Friendly alien assessment: " << nPage.getNPC_a() << endl;

                    outF.close();
                    
                    //txt file name stored to vector 
                    PN_save.push_back(TXT_file);

                    //reset variables used in printing the logbook onto new file
                    pReported = false;
                    gameMap.resetMap();
                    nPage.resetLB();
                    nPage.setP_Name();
                    nPage.setP_Diameter();
                    nPage.setCarry_Cap();


                    // spawn entities on map after resetting map, add to planet count (0 + 1 bc first planet), subtract fuel cost of travel
                    
                    //spawn NPC
                    int spawnAnyPos[2];
                    if ((rand() % 2 + 1) == 1){
                        int x = rand() % 2 + 1;
                        randPosSpawn(spawnAnyPos, x);
                        gameMap.spawnNPC(spawnAnyPos[0], spawnAnyPos[1]);
                    }

                    //spawn SITE
                    for (int i = 0; i < 7; i++){
                        int spawnAnyPos1[2];
                        int sTrait = rand() % 6 + 1;
                        int oneInTwo = rand() % 2 + 1;
                        randPosSpawn(spawnAnyPos1, oneInTwo);
                        int x = spawnAnyPos1[0];
                        int y = spawnAnyPos1[1];
                        while(gameMap.isFreeSpace(spawnAnyPos1[0], spawnAnyPos1[1]) == false){
                            oneInTwo = rand() % 2 + 1;
                            randPosSpawn(spawnAnyPos1, oneInTwo);
                        }
                        gameMap.spawnSite(spawnAnyPos1[0], spawnAnyPos1[1], sTrait);
                    }

                    //spawn MISF
                    int totalAmountMisf = rand() % 8 + 4;
                    for (int i = 0; i < totalAmountMisf-2; i++){
                        int spawnAnyPos2[2];
                        int oneInTwo = rand() % 2 + 1;
                        randPosSpawn(spawnAnyPos2, oneInTwo);
                        while(gameMap.isFreeSpace(spawnAnyPos2[0], spawnAnyPos2[1]) == false){
                            oneInTwo = rand() % 2 + 1;
                            randPosSpawn(spawnAnyPos2, oneInTwo);
                        }
                        int misf = rand() % 4 + 1;
                        gameMap.spawnMisfortune(spawnAnyPos2[0], spawnAnyPos2[1], misf);
                    }
                    for (int i = 0; i < 2; i++){
                        int spawnAnyPos3[2];
                        int oneInTwo = rand() % 2 + 1;
                        randPosSpawn(spawnAnyPos3, oneInTwo);
                        while(gameMap.isFreeSpace(spawnAnyPos3[0], spawnAnyPos3[1]) == false){
                            oneInTwo = rand() % 2 + 1;
                            randPosSpawn(spawnAnyPos3, oneInTwo);
                        }
                        gameMap.spawnMisfortune(spawnAnyPos3[0], spawnAnyPos3[1], 1);
                    }

                    //reset talk to alien
                    npc.setComms(false);
                    
                    //set change in fuel due to fuel cost
                    int d = user.getPlanetsExpl();
                    user.setPlanetsExplored(1);
                    int launCH = fuelcost_Recur(d);
                    user.setChangeAmountFuel(-launCH);
                    
                    //check for crewmate Sherry
                    int saveFuel = launCH/10;
                    if (user.getSelectCrewNames(0) == "Sherry" || user.getSelectCrewNames(1) == "Sherry"){
                        cout << "Your master Pilot Sherry saved you " << saveFuel << " fuel on his way to this planet (fuelused * 0.9).\n\n";
                        user.setChangeAmountFuel(saveFuel);
                    }
                }

                //User did not repot the planet
                else{
                    //print wormhole asci art
                    cout << '\n';
                    string W_line;
                    ifstream W_input;
                    W_input.open("wormhole.txt");
                    while(getline(W_input, W_line)){
                        cout << W_line << endl;
                    }
                    W_input.close();
                    cout << '\n';

                    //check if user rly wants to leave the planet
                    cout << "You are on a steady aproach to a the wormhole, are you sure you want to enter it? You will start a new page in your log book and you will lose fuel.\nEnter wormhole? [y/n]\n";
                    string yesno;
                    cin >> yesno;
                    //Input validation
                    while (yesno != "y" && yesno != "n"){
                        cout << "Invalid input. Re-enter input as [y/n].\n";
                        cin >> yesno;
                    }
                    if (yesno == "y"){
                        //reset site visits
                        for (int i = 0; i < 7; i++){
                            site_VisitAgain[i][0] = -1;
                            site_VisitAgain[i][1] = -1;
                        }

                        //reset variables set in while loop for main menu
                        pReported = false;
                        gameMap.resetMap();
                        nPage.resetLB();
                        nPage.setP_Name();
                        nPage.setP_Diameter();
                        nPage.setCarry_Cap();


                        // spawn entities on map after resetting map, add to planet count (0 + 1 bc first planet), subtract fuel cost of travel
                        //spawn NPC
                        int spawnAnyPos[2];
                        if ((rand() % 2 + 1) == 1){
                            int x = rand() % 2 + 1;
                            randPosSpawn(spawnAnyPos, x);
                            gameMap.spawnNPC(spawnAnyPos[0], spawnAnyPos[1]);
                        }

                        //spawn SITE
                        for (int i = 0; i < 7; i++){
                            int spawnAnyPos1[2];
                            int sTrait = rand() % 6 + 1;
                            int oneInTwo = rand() % 2 + 1;
                            randPosSpawn(spawnAnyPos1, oneInTwo);
                            int x = spawnAnyPos1[0];
                            int y = spawnAnyPos1[1];
                            while(gameMap.isFreeSpace(spawnAnyPos1[0], spawnAnyPos1[1]) == false){
                                oneInTwo = rand() % 2 + 1;
                                randPosSpawn(spawnAnyPos1, oneInTwo);
                            }
                            gameMap.spawnSite(spawnAnyPos1[0], spawnAnyPos1[1], sTrait);
                        }

                        //spawn MISF
                        int totalAmountMisf = rand() % 8 + 4;
                        for (int i = 0; i < totalAmountMisf-2; i++){
                            int spawnAnyPos2[2];
                            int oneInTwo = rand() % 2 + 1;
                            randPosSpawn(spawnAnyPos2, oneInTwo);
                            while(gameMap.isFreeSpace(spawnAnyPos2[0], spawnAnyPos2[1]) == false){
                                oneInTwo = rand() % 2 + 1;
                                randPosSpawn(spawnAnyPos2, oneInTwo);
                            }
                            int misf = rand() % 4 + 1;
                            gameMap.spawnMisfortune(spawnAnyPos2[0], spawnAnyPos2[1], misf);
                        }
                        for (int i = 0; i < 2; i++){
                            int spawnAnyPos3[2];
                            int oneInTwo = rand() % 2 + 1;
                            randPosSpawn(spawnAnyPos3, oneInTwo);
                            while(gameMap.isFreeSpace(spawnAnyPos3[0], spawnAnyPos3[1]) == false){
                                oneInTwo = rand() % 2 + 1;
                                randPosSpawn(spawnAnyPos3, oneInTwo);
                            }
                            gameMap.spawnMisfortune(spawnAnyPos3[0], spawnAnyPos3[1], 1);
                        }

                        //reset convo w freindly alien
                        npc.setComms(false);
                        int d = user.getPlanetsExpl();

                        //new planet = new fuel cost
                        user.setPlanetsExplored(1);

                        //set change in fuel due to fuel cost
                        int launCH = fuelcost_Recur(d);
                        user.setChangeAmountFuel(-launCH);
                        
                        //check for crewmate Sherry
                        int saveFuel = launCH/10;
                        if (user.getSelectCrewNames(0) == "Sherry" || user.getSelectCrewNames(1) == "Sherry"){
                            cout << "Your master Pilot Sherry saved you " << saveFuel << " fuel on his way to this planet (fuelused * 0.9).\n\n";
                            user.setChangeAmountFuel(saveFuel);
                        }
                    }
                }                
            }
        }

        
        /******USER QUITS THE GAME******/
        if (userChoiceMM == 7){
            //make sure user actually wants to give up
            char giveUpC;
            cout << "\033[31mAre you sure you want to give up? [y/n]" << endl;
            cin >> giveUpC;
            //check valid input
            while(giveUpC != 'y' && giveUpC != 'n'){
                //tell user it is invalid
                cout << "Invalid Input. Re-enter inputut as [y/n]." << endl;
                cin >> giveUpC;
            }
            cout << "\033[0m";
            //end game if user chooses y
            if(giveUpC == 'y'){
                ENDgameM = true;
            }            
        }


        // MAKE SURE PLAYER IS HEALTHY
        if (user.getPlayerH() <= 0.00999999999999 || user.getSuitH() <= 0.00999999999999){
            cout << '\n';
            string L_line;
            ifstream L_input;
            L_input.open("rage.txt");
            while(getline(L_input, L_line)){
                cout << "\033[31m" << L_line << endl;
            }
            L_input.close();
            cout << '\n';
            cout << "You died in space" << endl;
            
            cout << "\033[0m\nHit ENTER to continue." << endl;
            cin.ignore();
            while(true){
                if (cin.get() == '\n'){
                    break;
                }
            }
            ENDgameM = true;
        }


        // ALLOW USER TO READ ALL TEXT BEFORE MOVING ON
        if(ENDgameM == false && userChoiceMM != 1){
            cout << "\nHit ENTER to continue." << endl;
            cin.ignore(2, '\n');
            while(true){
                if (cin.get() == '\n'){
                    break;
                }
            }
        }
    }




    /*********** Game Ended: Phase 3 **********/
    
    for (int i = 0; i < 60; i++){
        cout << endl;
    }

    //print end txt file
    cout << '\n';
    ifstream E1input;
    string E1linef;
    E1input.open("end.txt");
    while (getline(E1input, E1linef)){
        cout << E1linef << endl;
    }
    E1input.close();
    cout << '\n';

    //print endgame txt
    char gOver[9] = {'G','A','M','E',' ','O','V','E','R'};
    for (int i = 0; i < 9; i++){
        cout << gOver[i];
        cout.flush();
        this_thread::sleep_for(chrono::milliseconds(110));
    }

    //continue to view results
    cout << "\n\nHit ENTER to view your score and your reported planet results." << endl;
    while(true){
        if (cin.get() == '\n'){
            break;
        }
    }


    // ((people saved in millions - people perished in millions) / 8000 * (planets accurately reported habitable + planets not-reported which were uninhabitable) / (planets explored)) * 100 
    int score = int( double(aliveH - deadH) / 8000.0 * double(user.getNumAccurateReport() + noREP_NH) / double(user.getPlanetsExpl()) * 100.0 );
    
    cout << "\n\n\nHere is a summary of how you did:\n";
    cout << "Planets explored: " << user.getPlanetsExpl() << endl;
    cout << "Planets accurately reported habitable: " << user.getNumAccurateReport() << endl;
    cout << "Planets inaccurately reported habitable: " << user.getNumInaccurateReport() << endl;
    cout << "Humans saved (in millions): " << aliveH << endl;
    cout << "Humans perished (in millions): " << deadH << endl;
    cout << "TOTAL SCORE: " << score << endl;
    //Highscore Result
    if (score == 100){
        cout << "You're a natural! You beat the game and got the maximum score.\n";
    }
    //regular result ask user to play again
    else{
        cout << "Play again to see if you can do better!\n";
    }

    //if vector size is not 0 then print to file.
    if (PN_save.size() > 0){
        if (PN_save.size() > 1){
            //BUBBLE SORT ALPHABETICALLY (PLANETNAMES which name the text files parallel to stored logbook pages)
            for (int i = 0; i < PN_save.size()-1; i++){     //technicaly iterating by two bc compare i and i + 1. so when i is at end it will be out of bounds so i never reaches end of vector
                for (int j = i+1; j < PN_save.size(); j++){     //j is i+1 so unlike above j can reach the last value in the PN_save size
                    string tem1 = PN_save[i];         //save vector at i and j to two temporary strings that are going to be compared
                    string tem2 = PN_save[j];         
                    //string comparison function writen outside main. low value -- high value char go as such: A-Z then 0-9
                    //compare char same position inside both temps
                    //perameter in order matters. when left string in vect has bigger alph value then swap

                    //go to line 167 for further referance
                    if(stringComparison(tem1, tem2) > 0){
                        swap(PN_save[i], PN_save[j]);
                    }
                }
            }
        }
        cout << "\nBellow is the logbook pages for planets you reported as habitable. The Pages are organized in numerical then alphabetical order by planet name.\n\n" << endl;
        //prints information from saved log text files to "results.txt" (gets name of file from vector)
        //ensure all files are printied by using for loop (iterate # of times = size of the vector)
        for (int i = 0; i < PN_save.size(); i++){
            string saveFName = PN_save[i];  //temp string holds the filename
        
            //user while get line to read file with logbook
            string lineP;   
            ifstream printInput;
            printInput.open(saveFName);
            while(getline(printInput, lineP)){
                cout << lineP << '\n';
            }
            cout << '\n' << endl;
            
            /**
             * use c_str() pointer to delete .txt file and prevent folder containing program to cluster up
             * does not use pointer in any other place and pointer does not point to functions; if Grading Instructor wishes to run code without pointer, it will work the same. feel free to comment this line and line with remove at bottom of code out
             * pointer is used for benifit of whoever is grading the project; allowing user to not see a mess of .txt files mixed in with .cpp files and .h files
             * 
             * c_str()
            **/
            remove(saveFName.c_str());
        }
    }
    else{
        cout << "\nYou did not report any planets as habitable therefore no logbook pages were saved." << endl;
    }


    //ends game and returns 0 for int main()
    cout << "\n\n\nHit ENTER when you are ready to exit the game." << endl;
    cout << "\n******YOU WILL NOT BE ABLE TO VIEW ANY INFORMATION/RESULTS FOR YOUR GAMEPLAY AFTER YOU CLICK ENTER******" << endl;
    
    while(true){
        if (cin.get() == '\n'){
            break;
        }
    }

    cout << endl;
    ifstream C1input;
    string C1linef;
    C1input.open("credits.txt");
    while (getline(C1input, C1linef)){
        cout << C1linef << endl;
    }
    C1input.close();

    //leave game by returning 0 for int main
    return 0;
}
// Credits: James McGregor & Parker Taranto
//
// cd Project3_McGregor_Taranto
// g++ -std=c++17 MainMenu.cpp hiddenMap.cpp friendlyAlien.cpp hostileAlien.cpp Player.cpp LogBook.cpp
// ./a.out
//
// It's Easter Sunday when writing this, so find it... (rough estimate)
// string findHA_Help(int Hint, int index, string HintAnswerTemp){
//     int Hint1 = Hint;
//     if (index < 3){
//         HintAnswerTemp += to_string(Hint%10);
//         Hint1 -= Hint%10;
//         Hint1 /= 10;
//         return findHA_Help(Hint1, index+1, HintAnswerTemp);
//     }
//     else{
//         return HintAnswerTemp;
//     }
// }
// string findHA(){
//     string HintAnswer = "";
//     string HintAnswerT = "";
//     string HintAnswerTT = "";
//     int idx = 0;
//     int FoundHint = ????;
//     HintAnswerTT = findHA_Help(FoundHint, idx, HintAnswerT);
//     for(int i = 2; i > -1; i--){
//         HintAnswer += HintAnswerTT[i];
//     }
//     return HintAnswer;
// }

/** (NOTES)
 * should have had everything cin to char instead of strings int double exc...
 * bugs out code if expecting int and gets char
 * all art asci art is from https://www.asciiart.eu/computers
**/

// may have simmilarities with some parts of Zoe Pekarek's project due to being her previous partner. Proffessors were coppied in an email of when I dropped her as a partner; I told her that she could keep what she had whick wasn't much.