#include "globals.h"
#include "helperFns.h"

// Return the character at x, y
chtype charAt(int x, int y) {
	//if(!mtx.try_lock())
	//	return false;
	//std::lock_guard<std::mutex> lock (mtx);
	mtx.lock();
	// check bounds
	if(x < 0 || y < 0)
		return 0;

	int curX, curY; // store current position; we will return to it

	// get value
	getyx(stdscr, curY, curX);
	chtype value = mvinch(y, x);

	// move back + return
	mvinch(curY, curX);
	mtx.unlock();
	return value;
}

bool writeAt(int x, int y, chtype letter) {
	//if(!mtx.try_lock())
		//return false;
	//std::lock_guard<std::mutex> lock(mtx);
	mtx.lock();
	// Check bounds
	if(x < 0 || y < 0)
		return false;
	
	int curX, curY;
	getyx(stdscr, curY, curX);


	mvinch(y, x);

	addch(letter);
	mvinch(curY, curX);
	mtx.unlock();
	return true;
}

bool writeAt(int x, int y, chtype letter, int color) {
	mtx.lock();
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

	mtx.unlock();
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
	std::string x;
	x += msg;
	mvprintw(20, 1, (x).c_str());
}
void printAtBottom(std::string msg) {
	mtx.lock();
	int x, y;
	getyx(stdscr, y, x);
	mvprintw(20, 1, msg.c_str());
	move(y,x);

	mtx.unlock();
}




// Game state
void winGame() {
	clear();
	printAtBottom("YOU WIN THE GAME!");
	refresh();
	GAME_WON = 1;
}

void loseGame() {
	clear();
	printAtBottom("YOU LOSE THE GAME!");
	refresh();
	GAME_WON = -1;
}





// check to see if the player can move there
bool isValid(int x, int y) {
	mtx.lock();
	// Within range of board
	if(y < 0 || x < 0)
		return false;
	

	// Move cursor, check character, move cursor back
	int curX, curY;
	getyx(stdscr, curY, curX);
	char testPos= mvinch(y, x);
	mvinch(curY, curX);


	// Now see if it's a valid spot
	if(WALLS.find(testPos) != WALLS.end())
	{
	//	cout << "NOT VALID" << endl;
		mtx.unlock();
		return false;
	}
	mtx.unlock();
	return true;	
}
