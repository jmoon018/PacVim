#include "globals.h"

int TOTAL_POINTS = 0;
int GAME_WON = 0;
std::string INPUT = "";
bool READY = false;
int LIVES = 3;
const int NUM_OF_LEVELS = 9;
bool ROGUE_KEYS = false;

double THINK_MULTIPLIER = 1.0;

int TOP = 0;
int BOTTOM = 0;
int WIDTH = 0;
int CURRENT_LEVEL = 0;

//int walllist[] = {'#', ACS_ULCORNER, ACS_LLCORNER, ACS_URCORNER,
//		ACS_LRCORNER, ACS_LTEE, ACS_RTEE, ACS_BTEE, ACS_TTEE,
//		ACS_HLINE, ACS_VLINE, ACS_PLUS};
int zz[] = {'#', 4194144, ACS_PLUS, 'b', 'c'};
//std::set<chtype> WALLS(zz, zz+5);
std::mutex mtx;
