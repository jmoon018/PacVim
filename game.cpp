#include <fstream>
#include <vector>
#include <cursesw.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <list>
#include <set>
using namespace std;



int wallist[] = {'#', ACS_ULCORNER, ACS_LLCORNER, ACS_URCORNER,
				ACS_LRCORNER, ACS_LTEE, ACS_RTEE, ACS_BTEE,
				ACS_TTEE, ACS_HLINE, ACS_VLINE, ACS_PLUS};

set<int> WALLS(wallist, wallist + sizeof(wallist)/sizeof(int));
// can be abstracted for ghosts or Player
struct avatar{
	int x;
	int y;
};


// check to see if the player can move there
bool isValid(int x, int y) {
	if(y < 0 || x < 0)
		return false;
	
	int curX, curY;
	getyx(stdscr, curY, curX);
	char testPos= mvinch(y, x);
	mvinch(curY, curX);

//	cout << "Cur char:" << ACS_DIAMOND << endl;
//	addch(4194400);

	if(WALLS.find(testPos) != WALLS.end())
	{
	//	cout << "NOT VALID" << endl;
		return false;
	}
	return true;	
}

void onMove(avatar& unit) {
	// delete any dots
	char curPos = inch();
	//cout << "OnMove.." << curPos << endl;
	if(curPos == '~') {
		addch(' ');
		move(unit.y, unit.x);
	}
}
void forcemove(int x, int y, avatar& unit) {
	//move(y, x);
	//cout << "Calling forcemove!" << endl;
	unit.x = x;
	unit.y = y;
	move(y, x);
	onMove(unit);
	//cout << "Unit.x = " <<  unit.x <<  "..Unit.y=" << unit.y << endl;
}

bool moveLeft(avatar& unit) {
	if(isValid(unit.x-1, unit.y)) {
		forcemove(unit.x-1, unit.y, unit); 
		return true;
	}
	else { return false; }
}

bool moveRight(avatar& unit) {
	if(isValid(unit.x+1, unit.y)) {
		forcemove(unit.x+1, unit.y, unit);
		return true;
	}
	else { return false; } 
}

bool moveDown(avatar& unit) {
	if(isValid(unit.x, unit.y+1)) {
		forcemove(unit.x, unit.y+1, unit);
		return true;
	}
	else { return false; }
}

bool moveUp(avatar& unit) {
	if(isValid(unit.x, unit.y-1)) {
		forcemove(unit.x, unit.y-1, unit);
		return true;
	}
	else { return false; }
}

int parseWordEnd(avatar& unit, bool isWord) {
	// Formula: Get next char, is it alphanumeric? If so, loop & break
	//		on nonalpha-, and viceversa. 
	// 2nd case: if you are not at the end of a word, loop until you
	//		reach a space

	// store the current character type
	char curChar = mvinch(unit.y, unit.x);
	bool isAlpha = isalnum(curChar);
	char nextChar = mvinch(unit.y, unit.x+1);
	mvinch(unit.y, unit.x);

	// breakOnSpace = true if the current character isn't the end of a word
	bool breakOnSpace = (nextChar != ' ' && curChar != ' ') ? true : false;
	bool breakOnAlpha = !isalnum(nextChar);
	while(true) { // no definite loop #; break when we reach conditions
		if(!breakOnAlpha == !isalnum(nextChar) && isWord) {
			break;
		}
		else if(breakOnSpace && nextChar == ' ') {
			break; 
		}
		else if(nextChar == '#') { // not allowed to go on # so break
			break;
		}
		else { // iterate
			if(!moveRight(unit))
				break;
			if(isalnum(nextChar))
				breakOnSpace = true;
			nextChar = mvinch(unit.y, unit.x+1);
			mvinch(unit.y, unit.x);
		}
	}
}
				
void parseWordBackward(avatar& unit, bool isWord) {
	// Formula: Get next char, is it alphanumeric? If so, loop & break
	//		on nonalpha-, and viceversa. 
	// 2nd case: if you are not at the end of a word, loop until you
	//		reach a space

	// store the current character type
	char curChar = mvinch(unit.y, unit.x);
	bool isAlpha = isalnum(curChar);
	char nextChar = mvinch(unit.y, unit.x-1);
	mvinch(unit.y, unit.x);

	// breakOnSpace = true if the current character isn't the end of a word
	bool breakOnSpace = (nextChar != ' ' && curChar != ' ') ? true : false;
	bool breakOnAlpha = !isalnum(nextChar);
	while(true) { // no definite loop #; break when we reach conditions
		if((!breakOnAlpha == !isalnum(nextChar)) && isWord) { 
			break;
		}
		else if(breakOnSpace && nextChar == ' ') {
			break; 
		}
		else if(nextChar == '#') { // not allowed to go on # so break
			break;
		}
		else { // iterate

			if(!moveLeft(unit))
				break;
			if(isalnum(nextChar))
				breakOnSpace = true;
			nextChar = mvinch(unit.y, unit.x-1);
			mvinch(unit.y, unit.x);
		}
	}
}

void parseWordForward(avatar& unit, bool isWord) {
	char curChar = mvinch(unit.y, unit.x);
	bool isAlpha = isalnum(curChar);
	char lastChar= 'X';
	mvinch(unit.y, unit.x); // move cursor back into place

	bool breakOnAlpha = !isalnum(curChar);
	
	while(true) {
		if((!isalnum(curChar) == !breakOnAlpha) && isWord ) { // if they are the same
			break;
		}
		else if(lastChar == ' ' && curChar != ' ') {
			break;
		}
		else if(lastChar == '#' || curChar == '#') {
			moveLeft(unit);
			break;
		}
		else {
			lastChar = curChar;
			if(!moveRight(unit))
				break;
			curChar = mvinch(unit.y, unit.x);
		}
	}
}


void gotoLine(avatar& unit, int line) {
	int wallCnt = 2;
	char curChar = mvinch(line, 0);
	while(wallCnt > 0) {
		}
	}

void onKeystroke(avatar& unit, string& key);

void getMore(avatar& unit, string& key) {
	char nextChar = getch();
	key += nextChar;
	onKeystroke(unit, key);
}

void onKeystroke(avatar& unit, string& key) {
	if(key == "q") {
		endwin();
	}
	else if(key == "h") {
		moveLeft(unit);
	}
	else if(key == "j") {
		moveDown(unit);
	}
	else if(key == "k") {
		moveUp(unit); 
	}
	else if(key == "l") {
		moveRight(unit);
	}
	else if(key == "r") {
		refresh();	
	}
	else if(key == "g") {
		getMore(unit, key);
	}
	else if(key == "w") {
		parseWordForward(unit, true); 
	}
	else if(key == "W") {
		parseWordForward(unit, false);	
	}
	else if(key == "b") {
		parseWordBackward(unit, true);
	}
	else if(key == "B") {
		parseWordBackward(unit, false);
	}
	else if(key == "E") {
		parseWordEnd(unit, false);
	}
	else if(key == "e") {
		parseWordEnd(unit, true);
	}
	else if(key == "gg") {
		gotoLine(unit,1);
	}

	key = "";
	refresh();
}
//   +.x.x+x.+      

void drawScreen(const char* file) {
	clear();
	ifstream in(file);

	vector<string> maze;
	vector<string> fixedMaze;
	string str;
	while(getline(in, str)) {
		maze.push_back(str + "\n");
		fixedMaze.push_back(str + "\n");
	}
	in.close();
	
	for(int i = 0; i < maze.size(); i++)
	{
		str = maze.at(i);
		int length = (int) maze.at(i).length();
		for(int j = 0; j < length-1; j++)
		{
		/*	if(maze.at(i).at(j) == ' ')
			{
				addch(ACS_BULLET);
				continue;
			}
		*/
			addch(maze.at(i).at(j));
			continue;
			//cout << "Len: " << length; 
			//cout << "i IS " << i << "..J is: " << j << endl;
			bool left = false, right = false,
				up = false, down = false;
			char* ch = &( fixedMaze.at(i).at(j));
			//cout << *ch << flush ;	
			// Check left
			if((j - 1) >= 0) {
				if(str.at(j-1) == '#') {
					left = true;
				}
			}
			// Check right
			if((j+1) < (length-1)) {
				if(str.at(j + 1) == '#') {
					right = true;
				}
			}
			//cout << "Up.." << endl;
			// Check up
			if((i - 1) >= 0) {
				if(maze.at(i - 1).at(j) == '#') {
					up = true;
				}
			}
			//cout << "Down.." << endl;
			// Check down
			if((i+2) < (fixedMaze.size())) {
				if(maze.at(i+1).at(j) == '#') {
					down = true;
				}
			}
                                	
			/*
			if(left && right && up && down)
				addch(ACS_PLUS); 
			else if(left && right && up)
				addch(ACS_BTEE); 
			else if(left && right && down)
				addch(ACS_TTEE); 
			else if(left && up && down)
				addch(ACS_RTEE); 
			else if(right && up && down)
				addch(ACS_LTEE); 
			else if(up && left)
				addch(ACS_LRCORNER); 
			else if(up && right)
				addch(ACS_LLCORNER); 
			else if(down && left)
				addch(ACS_URCORNER); 
			else if(down && right)
				addch(ACS_ULCORNER); 
			else if(down || up)
				addch(ACS_VLINE); 
			else 
				addch(ACS_HLINE); 
			
			*/
			addch('#');
		}
		addch('\n');
		//printf(maze.at(i).c_str());
		//cout << endl;
		//cout << "I is: " << i << endl;
		refresh();
	}
	refresh();
}



void test() {
	addch(ACS_LLCORNER);
	cchar_t theChar;
	//int x = in_wch(&theChar);
//	cout << "CHAR: " << (char) theChar;
}

int main(int argc, char** argv)
{
	// Setup
	setlocale(LC_ALL, "");
	WINDOW* win = initscr();
	noecho(); // dont print anything to the screen
	clear();
	refresh();

	refresh();
	move(5, 5);
	refresh();


	avatar player;
	player.x = 5; player.y = 5;


	drawScreen("map1.txt");
	
	printw("Horizontal LINE               "); addch(ACS_HLINE); printw("  ACS_HLINE\n");
	
	while(true) {
		string key;
		key += getch();
		if(key != "")
			onKeystroke(player, key);
		refresh();
		int x, y;
		getyx(win, y, x);
		//cout << "Y: " << y << "..X:" << x << endl;
	}	


	sleep(3);
	endwin();
	return 0;
}



      
      
