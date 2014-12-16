#ifndef GLOBALS_H
#define GLOBALS_H

#include <set>
#include <mutex>
extern int TOTAL_POINTS;
extern int GAME_WON; // 0 = in progress, 1 = won, -1 = lose

extern bool THINKING; //

extern std::set<int> WALLS;
extern std::mutex mtx;
#endif

