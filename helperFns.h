#ifndef HELPERFNS_H
#define HELPFNS_H

#include <queue>
/* HELPER FUNCTIONS */


/* Write/Read Manager
 When the game is being played, sometimes weird
 characters will appear randomly. When multithreading is not
 used, this bug disappears. I believe the characters appear
 because the read/write methods are being used at the same time
 (from concurrent threads), which can lead to weird behavior,
 
 With WR manager, all these methods (charAt, writeAt, etc) are done
 sequentially, the order determined by a priority queue. The player's
 keystrokes take priority.
 */

// function pair
template <typename T>
struct funct_pair{
	T fn;
	int priority;
};

// Allow comparision for the funct_pair
template <typename T>
bool operator<(const funct_pair<T> &a, const funct_pair<T> &b) {
	return a.priority < b.priority;
}

template <typename T>
class WRManager {
	private:
		// vector is used for priority queue by default
		std::priority_queue<funct_pair<T>> pq;
		void execute();
		bool working;
	public:
		WRManager();
		void add(funct_pair<T>);
};

// execute
// Return: none
// Param: none
// Purpose: calls the function from funct_pair that is highest
//		in the priority queue. It will call execute again if
//		there are more funct_pairs in the queue or when add is called
template <typename T>
void WRManager<T>::execute() {
	cout << "Executing.." << endl ;
	working = true;
	funct_pair<T> fp = pq.top();
	fp.fn();
	pq.pop();
	if(pq.empty())
		execute();
	working = false;
}

template <typename T>
void WRManager<T>::add(funct_pair<T> fp) {
	cout << "Adding" << endl;
	pq.push(fp);
	if(!working)
		execute();
}

template <typename T>
WRManager<T>::WRManager() {
	//pq = std::priority_queue<T>;
	working = false;
}


// Return the character at x, y
chtype charAt(int x, int y) {
	// check bounds
	if(x < 0 || y < 0)
		return 0;

	int curX, curY; // store current position; we will return to it

	// get value
	getyx(stdscr, curY, curX);
	chtype value = mvinch(y, x);

	// move back + return
	mvinch(curY, curX);
	return value;
}

bool writeAt(int x, int y, char letter, int color = COLOR_WHITE) {
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
	return true;
}

void printAtBottomChar(char msg) {
	std::string x;
	x += msg;
	mvprintw(20, 1, (x).c_str());
}
void printAtBottom(std::string msg) {
	int x, y;
	getyx(stdscr, y, x);
	mvprintw(20, 1, msg.c_str());
	move(y,x);
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
		return false;
	}
	return true;	
}

#endif
