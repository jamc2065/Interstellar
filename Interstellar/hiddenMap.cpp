
#include <iostream>
#include <cstring>
#include "hiddenMap.h"
using namespace std; 

// different than the given map class.  most of the changes are related to hiding all spawns

hiddenMap::hiddenMap()
{
    resetMap();
}

// resets player position, count values, and initializes values in position arrays to -1
void hiddenMap::resetMap() {
    playerPosition[0] = 0; 
    playerPosition[1] = 0;

    npcPosition[0] = -1;
    npcPosition[1] = -1;

    npc_on_map = false;
    misfortune_count = 0;
    site_count = 0;

    for (int i = 0; i < num_misfortunes; i++) {
        misfortunes[i][0] = -1;
        misfortunes[i][1] = -1;
        misfortunes[i][2] = -1;
    }

    for (int i = 0; i < num_sites; i++) {
        sites[i][0] = -1;
        sites[i][1] = -1;
        sites[i][2] = -1;
    }

    npcPosition[0] = -1;
    npcPosition[1] = -1;

    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            mapData[i][j] = '-';
        }
    }
}


// return player's row position
int hiddenMap::getPlayerRowPosition() {
    return playerPosition[0];
}

// return player's column position
int hiddenMap::getPlayerColPosition() {
    return playerPosition[1];
}

int hiddenMap::getSiteCount() {
    return site_count;
}

int hiddenMap::getMisfortuneType(){
    for(int i = 0; i < misfortune_count; i++){
        if (playerPosition[0] == misfortunes[i][0] && playerPosition[1] == misfortunes[i][1]){
            if (misfortunes[i][2] != -1){
                return misfortunes[i][2];
            }
        }
    }
    return 0;
}

int hiddenMap::getMisfortuneCount() {
    return misfortune_count;
}

int hiddenMap::getSiteTrait() {
    for(int i = 0; i < num_sites; i++){
        if (playerPosition[0] == sites[i][0] && playerPosition[1] == sites[i][1]){
            if (sites[i][2] != -1){
                return sites[i][2];
            }
        }
    }
    return 0;
}

bool hiddenMap::isNPCLocation(){
    if (playerPosition[0] == npcPosition[0] && playerPosition[1] == npcPosition[1] && npc_on_map == true){
        return true;
    }
    return false;
}

bool hiddenMap::isMisfortuneLocaton(){
    for(int i = 0; i < misfortune_count; i++){
        if (playerPosition[0] == misfortunes[i][0] && playerPosition[1] == misfortunes[i][1]){
            if (misfortunes[i][2] == -1){
                return false;
            }
            return true;
        }
    }
    return false;
}

bool hiddenMap::isSiteLocation(){
    for(int i = 0; i < num_sites; i++){
        if (playerPosition[0] == sites[i][0] && playerPosition[1] == sites[i][1]){
            if (sites[i][2] == -1){
                return false;
            }
            return true;
        }
    }
    return false;
}

bool hiddenMap::isHabitable(){
    bool water = false;
    bool oxygen = false;
    bool fertile_soil = false;
    int non_habitable_traits = 0;
    for (int i = 0; i < num_sites; i++){
        if (sites[i][2] == 1){
            water = true;
        } 
        else if (sites[i][2] == 2){
            oxygen = true;
        }
        else if (sites[i][2] == 3){
            fertile_soil = true;
        }
        else if (sites[i][2] >= 4 && sites[i][2] <= 6){
            non_habitable_traits++;
        }
    }
    if (water && oxygen && fertile_soil && (non_habitable_traits <= 1)){
        habitable = true;
    }
    else{
        habitable = false;
    }
    return habitable;
}

bool hiddenMap::isFreeSpace(int row, int col){
    if (!(row >= 0 && row < num_rows && col >= 0 && col < num_cols)) {
        return false;
    }
    for(int i = 0; i < misfortune_count; i++){
        if (row == misfortunes[i][0] && col == misfortunes[i][1]){
            return false;
        }
    }
    for(int i = 0; i < num_sites; i++){
        if (row == sites[i][0] && col == sites[i][1]){
            return false;
        }
    }
    if (row == npcPosition[0] && col == npcPosition[1]){
        return false;
    }
    return true;
}


void hiddenMap::setMisfortuneCount(int num_misfortunes){
    misfortune_count = num_misfortunes;
}

void hiddenMap::setNPC(bool isNpc){
    npc_on_map = isNpc;
}

void hiddenMap::setPlayerRowPosition(int row) {
    playerPosition[0] = row;
}

void hiddenMap::setPlayerColPosition(int col) {
    playerPosition[1] = col;
}



bool hiddenMap::spawnNPC(int row, int col) {
    if (npc_on_map) {
        return false;
    }

    if (!isFreeSpace(row, col)) {
        return false;
    }

    npcPosition[0] = row;
    npcPosition[1] = col;
    npc_on_map = true;
    return true;
}

bool hiddenMap::spawnMisfortune(int row, int col, int type) {

    if (misfortune_count >= num_misfortunes) {
        return false;
    }

    // location must be blank to spawn
    if (!isFreeSpace(row, col)) {
        return false;
    }

    if (misfortunes[misfortune_count][0] == -1 && misfortunes[misfortune_count][1] == -1) {
        misfortunes[misfortune_count][0] = row;
        misfortunes[misfortune_count][1] = col;
        misfortunes[misfortune_count][2] = type;
        misfortune_count++;
        return true;
    }

    return false;
}

bool hiddenMap::spawnSite(int row, int col, int type) {

    if (site_count >= num_sites) {
        return false;
    }

    // location must be blank to spawn
    if (!isFreeSpace(row, col)) {
        return false;
    }

    if (sites[site_count][0] == -1 && sites[site_count][1] == -1) {
        sites[site_count][0] = row;
        sites[site_count][1] = col;
        sites[site_count][2] = type;
        site_count++;
        return true;
    }

    return false;
}

void hiddenMap::revealMisfortune(int row, int col) {
    mapData[row][col] = '*';
}

void hiddenMap::revealSite(int row, int col) {
    mapData[row][col] = 'S'; //moved from spawnMistfortune (to keep hidden until player hits)
}

void hiddenMap::revealNpc(int row, int col) { //moved from spawnNPC (to keep hidden until player hits)
    mapData[row][col] = 'A';
}

bool hiddenMap::executeMove(char move){
    // if user inputs w, move up if it is an allowed move
    if(!(playerPosition[0] == 0) && (tolower(move) == 'w')){
        playerPosition[0] -= 1;
        return true; 
    }
    // if user inputs s, move down if it is an allowed move
    else if(!(playerPosition[0] == (num_rows - 1)) && (tolower(move) == 's')){
        playerPosition[0] += 1;
        return true; 
    }
    // if user inputs a, move left if it is an allowed move
    else if(!(playerPosition[1] == 0) && (tolower(move) == 'a')){
        playerPosition[1] -= 1;
        return true; 
    }
    // if user inputs d, move right if it is an allowed move
    else if(!(playerPosition[1] == (num_cols - 1)) && (tolower(move) == 'd')){
        playerPosition[1] += 1;
        return true; 
    }
    else{
        cout << "Invalid Move." << endl; 
        return false;
    }
}

void hiddenMap::displayMap() {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if (playerPosition[0] == i && playerPosition[1] == j) {
                cout << "X";
            }
            else {
                cout << mapData[i][j];
            }
        }
        cout << endl;
    }
}