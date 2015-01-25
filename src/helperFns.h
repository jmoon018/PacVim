#ifdef __APPLE__
#include <curses.h>
#else
#include <cursesw.h>
#endif


#ifndef HELPERFNS_H
#define HELPERFNS_H

#include "globals.h"
//#include <ncursesw/cursesw.h>
#include <fstream>
#include <string>

// Return the character at x, y
chtype charAt(int x, int y);
bool writeAt(int x, int y, chtype letter);
bool writeAt(int x, int y, chtype letter, int color);
void writeError(std::string msg);
void printAtBottomChar(char msg);
void printAtBottom(std::string msg);



// Game state
void winGame();
void loseGame();

// 
bool isInside(int x, int y, std::string direction);

// check to see if the player can move there
bool isValid(int x, int y);

#endif
