#ifndef GLOBALS_H
#define GLOBALS_H

#include <set>
#include <mutex>
#include <ncursesw/cursesw.h>
extern int TOTAL_POINTS;
extern int GAME_WON; // 0 = in progress, 1 = won, -1 = lose
extern std::string INPUT; // keyboard characters
extern int CURRENT_LEVEL;
extern int LIVES;
extern const int NUM_OF_LEVELS;
extern bool ROGUE_KEYS;

extern bool READY;
extern double THINK_MULTIPLIER; // all the think times for the AI are multipled by this


extern int TOP;
extern int BOTTOM;
extern int WIDTH;
#define HEIGHT (TOP - BOTTOM)

//extern std::set<chtype> WALLS;
extern std::mutex mtx;
#endif

