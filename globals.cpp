#include "globals.h"

int TOTAL_POINTS = 0;
int GAME_WON = 0;
bool THINKING = false;

int walllist[] {'#'};
std::set<int> WALLS(walllist, walllist + sizeof(walllist)/sizeof(int));
std::mutex mtx;

