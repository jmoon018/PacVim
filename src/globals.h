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
#include <cstddef>
#include <ncurses.h>
#endif



#ifndef GLOBALS_H
#define GLOBALS_H

#include <set>
#include <mutex>
//#include <cursesw.h>
extern int TOTAL_POINTS;
extern int GAME_WON; // 0 = in progress, 1 = won, -1 = lose
extern std::string INPUT; // keyboard characters
extern int CURRENT_LEVEL;
extern int LIVES;
extern const int NUM_OF_LEVELS;

extern bool READY;
extern double THINK_MULTIPLIER; // all the think times for the AI are multipled by this


extern int TOP;
extern int BOTTOM;
extern int WIDTH;
#define HEIGHT (TOP - BOTTOM)

extern std::mutex mtx;
#endif

#ifndef MAPS_LOCATION
#define MAPS_LOCATION "maps"
#endif
