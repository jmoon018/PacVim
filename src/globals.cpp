#include "globals.h"
#include <string>

int TOTAL_POINTS = 0;
int GAME_WON = 0;
std::string INPUT = "";
bool READY = false;
int LIVES = 3;
const int NUM_OF_LEVELS = 9;

double THINK_MULTIPLIER = 1.0;

int TOP = 0;
int BOTTOM = 0;
int WIDTH = 0;
int CURRENT_LEVEL = 0;

std::mutex mtx;
