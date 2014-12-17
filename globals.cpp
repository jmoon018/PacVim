#include "globals.h"

int TOTAL_POINTS = 0;
int GAME_WON = 0;
bool THINKING = false;
std::string INPUT = "";

int CURRENT_LEVEL = 0;

int walllist[] {'#'};
std::set<int> WALLS(walllist, walllist + sizeof(walllist)/sizeof(int));
std::mutex mtx;

