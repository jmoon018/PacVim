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

#include "globals.h"
#include "helperFns.h"
#include <sstream>
#include <unistd.h>

int stuff[] = {'#', static_cast<int>(ACS_ULCORNER), static_cast<int>(ACS_LLCORNER), static_cast<int>(ACS_URCORNER), static_cast<int>(ACS_LRCORNER),
	static_cast<int>(ACS_LTEE), static_cast<int>(ACS_RTEE), static_cast<int>(ACS_BTEE), static_cast<int>(ACS_TTEE), static_cast<int>(ACS_HLINE), static_cast<int>(ACS_VLINE), static_cast<int>(ACS_PLUS)};
std::set<int> WALLS(stuff, stuff + 12);
// Return the character at x, y
chtype charAt(int x, int y) {
	//if(!mtx.try_lock())
	//	return false;
	//std::lock_guard<std::mutex> lock (mtx);
	//mtx.lock();
	// check bounds
	if(x < 0 || y < 0)
		return 0;

	int curX, curY; // store current position; we will return to it

	// get value
	getyx(stdscr, curY, curX);
	chtype value = mvinch(y, x);

	// move back + return
	mvinch(curY, curX);
	//mtx.unlock();
	return value;
}

bool writeAt(int x, int y, chtype letter) {
	//if(!mtx.try_lock())
		//return false;
	//std::lock_guard<std::mutex> lock(mtx);
	//mtx.lock();
	// Check bounds
	if(x < 0 || y < 0)
		return false;

	int curX, curY;
	getyx(stdscr, curY, curX);


	mvinch(y, x);

	addch(letter);
	mvinch(curY, curX);
	//mtx.unlock();
	return true;
}

bool writeAt(int x, int y, chtype letter, int color) {
	//mtx.lock();
	// Check bounds
	if(x < 0 || y < 0)
		return false;

	int curX, curY;
	getyx(stdscr, curY, curX);


	mvinch(y, x);
	attron(COLOR_PAIR(color));
	addch(letter);
	attroff(COLOR_PAIR(color));
	mvinch(curY, curX);

	//mtx.unlock();
	return true;
}

void writeError(std::string msg) {
	std::ofstream fs;
	fs.open("errors.log", std::fstream::app);
	fs << msg;
	fs << "\n";
	fs.close();
}

void printAtBottomChar(char msg) {
	//mtx.lock();
	std::string x;
	x += msg;
	mvprintw(TOP+5, 0, (x).c_str());
	//mtx.unlock();
}
void printAtBottom(std::string msg) {
	//mtx.lock();
	int x, y;
	getyx(stdscr, y, x);
	mvprintw(TOP+1, 1, msg.c_str());
	mvinch(y,x);
	move(y,x);

	//mtx.unlock();
}




// Game state
void winGame() {
	clear();
	writeError("YOU WIN");

	if((CURRENT_LEVEL % 3) == 0) {
		printAtBottom("YOU WIN THE GAME!\nGAIN A LIFE!");
	}
	else {
		printAtBottom("YOU WIN THE GAME!");
	}

	refresh();
	GAME_WON = 1;
	READY = false;
	sleep(1);
}

void loseGame() {
	clear();
	writeError("YOU LOSE");
	printAtBottom("YOU LOSE THE GAME!\nLOST 1 LIFE");
	refresh();
	GAME_WON = -1;
	READY = false;

	LIVES--;
	sleep(1);
}





// check to see if the player can move there
bool isValid(int x, int y) {
	// Within range of board
	if(y < 0 || x < 0)
		return false;
	

	// Move cursor, check character, move cursor back
	int curX, curY;
	getyx(stdscr, curY, curX);
	chtype testPos= mvinch(y, x);
	mvinch(curY, curX);


	// Now see if it's a valid spot
	if(testPos >= 4000000 || WALLS.find(testPos) != WALLS.end())
	{
		return false;
	}
	return true;	
}

// recursive
bool isInside(int x, int y, std::string direction = "omni") {
	// we can tell if a location is bounded by the the walls
	// if we can look up, right, left, down and find a wall before
	// hitting the edge of the screen (eg: the far top or far
	// left of the console) and before the width value is exceeded

	// out of screen or bounds -- return false
	if(x <  0 || y < 0 || x > WIDTH || y > HEIGHT) {
		return false;
	}
	chtype value = charAt(x, y);

	// found a wall
	if(value >= 4000000) { 
		return direction != "omni"; // can't call isInside(x,y, omni) on a wall
	}

 	// no wall found -- continue searching in the proper direction
	if(direction == "left") { 
		return isInside(x-1, y, "left");
	}
	else if(direction == "right") {
		return isInside(x+1, y, "right");
	}
	else if(direction == "up") {
		return isInside(x, y-1, "up"); 
	}
	else if(direction == "down") {
		return isInside(x, y+1, "down");
	}
	else {
		return isInside(x+1, y, "right") && isInside(x-1, y, "left") &&
			isInside(x, y-1, "up") && isInside(x, y+1, "down");
	}
}

	



	
