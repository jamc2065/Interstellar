------------------------
HOW TO COMPILE AND RUN
------------------------
Go to file:     cd Project3_McGregor_Taranto

Compile:        g++ -std=c++17 MainMenu.cpp hiddenMap.cpp friendlyAlien.cpp hostileAlien.cpp Player.cpp LogBook.cpp

Run:            ./a.out

------------------------
DEPENDENCIES
------------------------
removes files that are temporarily 
stored in folderwith use of remove
function and pointers. If this use
of pointers is found to be cheating 
please comment out the use of remove
code should run the same just txt 
files will not be deleted from folder
after game ends. remove is only used
to improve organization of folder

------------------------
SUBMISSION INFORMATION
------------------------
CSCI 1300 Spring 2022
Author: James McGregor, Parker Taranto
Recitation: 123 – Jot, 204 - Koushik
Date: March 28, 2022
Project 3 -- hides misfortunes

------------------------
ABOUT THIS PROJECT
------------------------
This project develops a unique version of the 
Interstellar Project described on gitHub. The
game takes the user on an adventure similar
to the Interstellar movie. The user is presented
with many options and choices as they decide if 
a planet is habitable or not. The user can report
habitable planets and populate them with humans
from earth if they indeed are habitable. When 
exploring planets the user will encounter 
enemies, misfortunes, friendly aliens and sites
where they will gain information about a 
given planet. The user will end the game if 
all humans have left earth, if their space suit
or player health reach 0, or if the user choses
to manually leave the game. At the end of the
game the user will be presented with their game
score and their saved logbook with which will 
contain all planets that they reported throughout
the game.