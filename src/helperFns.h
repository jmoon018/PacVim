/*

Copyright 2015 Jamal Moon

PacVim is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License (LGPL) as 
published by the Free Software Foundation, either version 3 of the 
License, or (at your option) any later version.

PacVim program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */
#ifdef __APPLE__
#include <curses.h>
#elif __FreeBSD__
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
