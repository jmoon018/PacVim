// TODO: TEN LEVELS
// LOOP TO BEGINNING AFTER 10TH GAME
//	THEN MAKE GHOSTS FASTER
// MORE VIM COMMANDS (numbers, gg, G, $, 0, ^)
// beta test it w/ others 

#include <vector>
#include <iostream>

#include "globals.h"
#include "helperFns.h"
#include "avatar.h"
#include "ghost1.h"

using namespace std;

// DIMENSIONS
int HEIGHT;
int WIDTH;
void gotoLine(avatar& unit, int line) {
	int wallCnt = 2;
	char curChar = mvinch(line, 0);
	while(wallCnt > 0) {
		}
}

void gotoLineBeginning(int line, avatar &unit) {
	int x = 0;
	string str = "LOL: ";
	while(mvinch(x, line) == '#' ) {
		x++;
		writeError("LLLLLLL");
	}
	unit.moveTo(x, line);
}

void onKeystroke(avatar& unit, char key);

void getMore(avatar& unit, char key) {
	char nextChar = getch();
	onKeystroke(unit, key);
}


bool isFullDigits(string &str) {
	for(unsigned i = 0; i < str.size(); i++) {
		if(!isdigit(str[i]))
			return false;
	}
	return true;
}

void doKeystroke(avatar& unit) {
	if(INPUT== "q") { // allow ctrl c to exit game properly
		endwin();
	}
	else if(INPUT == "h") {
		unit.moveLeft();
	}
	else if(INPUT == "j") {
		unit.moveDown();
	}
	else if(INPUT == "k") {
		unit.moveUp(); 
	}
	else if(INPUT == "l") {
		unit.moveRight();
	}
	else if(INPUT == "r") {
		refresh();	
	}
	else if(INPUT == "w") {
		unit.parseWordForward(true); 
	}
	else if(INPUT == "W") {
		unit.parseWordForward(false);	
	}
	else if(INPUT == "b") {
		unit.parseWordBackward(true);
	}
	else if(INPUT == "B") {
		unit.parseWordBackward(false);
	}
	else if(INPUT == "E") {
		unit.parseWordEnd(false);
	}
	else if(INPUT == "e") {
		unit.parseWordEnd(true);
	}
	else if(INPUT == "$") { 
		unit.parseToEnd(); 
	}
	else if(INPUT == "0") {
		unit.parseToBeginning();
	}
	else if(INPUT == "gg") {
		gotoLineBeginning(1, unit);	
	}
	else if(INPUT == "^") {
		// goes to first character after blank
		unit.parseToBeginning();
		unit.parseWordForward(true);
	}
	else if(INPUT == "&") {
		GAME_WON = 1; // l337 cheetz
	}
}	

void onKeystroke(avatar& unit, char key) {
	THINKING = true;

	// there are some weird edge cases which I want to handle here:
	// 1. #G [moves to line #]
	// 2. 1G = same as gg
	// 3. gg = beginning of file... it's weird bc it's two non-digit characters

	// If INPUT != empty, and the user inputs a number, INPUT
	// should reset.. eg: 3g3 dd = 1 dd, not 3 dd
	string we = "KEYSTROKE: " + key;
	//writeError("KEYSTROKE");
	if(key == 'g') { 
		if(INPUT.empty() || INPUT.size() == 1 && INPUT[0] == 'g') {	
			
			INPUT += key;
			doKeystroke(unit);
		}
		else {
			INPUT = "";
		}
	}
	else if(!INPUT.empty() && isdigit(key)) {
		//writeError("resetting..");
		// reset it.. can't enter a digit in the middle of input
		INPUT = "";
	}
	// we have full digits and then enter a character
	else if(!INPUT.empty() && isFullDigits(INPUT) && !isdigit(key)) { 
		//writeError("num -> char");
		int num = std::stoi(INPUT, nullptr, 0); // extracts 33 from 33dd for example

		if(key == 'G') {
			// go to line num
			gotoLine(unit, num);
			THINKING = false;
			INPUT = "";
			refresh();
			return;
		}

		INPUT = key; 
		for(int i = 0; i < num; i++) {
			doKeystroke(unit);
		}
		INPUT = "";
	}
	else {
		//writeError("doing function");
		INPUT += key;
		if(INPUT == "0" || !isFullDigits(INPUT)) {
			doKeystroke(unit);
			INPUT = "";
		}
	}


	refresh();
	THINKING = false;
//	mtx.unlock();
}

void levelMessage() {
	// find appropriate message
	string msg;
	if(CURRENT_LEVEL == 0)
		msg = "LEVEL 0";
	else if(CURRENT_LEVEL == 1) {
		msg = "LEVEL 1";
	}
	else if(CURRENT_LEVEL == 2) {
		msg = "LEVEL 2";
	}
	else if(CURRENT_LEVEL == 3) {
		msg = "LEVEL 3";
	}

	printw(msg.c_str());
	refresh();
	usleep(1500000);

	// clear and reset everything
	clear();
	move(0,0);
	usleep(100000);
	refresh();
}

void drawScreen(const char* file) {
	levelMessage();
	clear();
	ifstream in(file);

	vector<vector <chtype> > board;
	vector<vector <chtype> > fixedBoard;
	string str;
	vector<chtype> line;

	while(getline(in, str)) {
		for(unsigned i = 0; i < str.length(); i++) {
			line.push_back(str[i]);
		}
		board.push_back(line);
		fixedBoard.push_back(line);
		line.clear();
	}
	in.close();
	for(unsigned i = 0; i < board.size(); i++) {
		unsigned length = board.size();
		for(unsigned j = 0; j < board.at(i).size(); j++) {
			
			if(board.at(i).at(j) != '~' && 
				board.at(i).at(j) != ' ' &&  board.at(i).at(j) != '#') 
				TOTAL_POINTS++;

			//cout << "Len: " << length; 
			//cout << "i IS " << i << "..J is: " << j << endl;
			bool left = false, right = false,
				up = false, down = false;
			chtype* ch = &( fixedBoard.at(i).at(j));
			//cout << *ch << flush ;	
			// Check left
			writeError("1");
			//cout << "J: " << j << endl;
			//cout << "LEL" << endl;
			if(j >= 1) {
				//cout << "doing left.. " << endl;
				if(board.at(i).at(j-1) == '#') {
					left = true;
				}
			}
			// Check right
			//cout << "J: " << j;
			if((j+1) < (board.at(i).size())) {
				if(board.at(i).at(j + 1) == '#') {
					right = true;
				}
			}
			//cout << "Up.." << endl;
			// Check up
			if(i >= 1) {
				writeError("I is ... ");
				if(board.at(i - 1).at(j) == '#') {
					up = true;
				}
			}

			//cout << "Down.." << endl;
			// Check down
			if((i+2) < (fixedBoard.size())) {
				if(board.at(i+1).at(j) == '#') {
					down = true;
				}
			}
                                	
		
			if(*ch == '#') {
				attron(COLOR_PAIR(3));
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
				attroff(COLOR_PAIR(3));
			}
			else {
				if(*ch == '~') {
					attron(COLOR_PAIR(6));
				}
				addch(*ch);
				attroff(COLOR_PAIR(6));
			}
			
			
		//	addch('#');
		}
		addch('\n');
		//printf(maze.at(i).c_str());
		//cout << endl;
		//cout << "I is: " << i << endl;
	}
	refresh();

	HEIGHT= fixedBoard.size();
	WIDTH = fixedBoard.at(0).size();
}



void test() {
	addch(ACS_LLCORNER);
	cchar_t theChar;
	//int x = in_wch(&theChar);
//	cout << "CHAR: " << (char) theChar;
}

void printIt(string msg, avatar& player) {
	move(20, 0);
	printf(msg.c_str());
	//move(player.y, player.x);
}


void defineColors() {
	start_color();
	init_color(COLOR_CYAN, 1000, 500, 500);
	init_pair(1, COLOR_RED	, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE	, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
}


void playGame(avatar &player) {
	char key;
	while(key != 'q' && GAME_WON == 0) {
		key = getch();
		if(key != 'q')
			onKeystroke(player, key);
		stringstream ss;
		ss << "Points: " << player.getPoints() << "/" << TOTAL_POINTS << "\n";
		if(GAME_WON == 0)
			printAtBottom(ss.str());
		move(player.getY(), player.getX());
		refresh();
	}	
	printf("AHHH");
	
	//ghostThread1.join();
	//ghostThread2.join();

	clear();
	if(GAME_WON == 1) {
		winGame();
	}
	else {
		loseGame();
	}
}


void init(const char* mapName, int ghostCnt, double thinkMultiplier) {
	mtx.lock();
	// set up map
	clear();
	drawScreen(mapName);
	mtx.unlock();

	// create ghosts and player
	avatar player (5, 6, true);
	
	Ghost1 ghost1(1, 1, 1 * thinkMultiplier, COLOR_RED);
	Ghost1 ghost2(10, 1, 0.8 * thinkMultiplier, COLOR_RED);
	Ghost1 ghost3(15, 1, 0.9  * thinkMultiplier, COLOR_RED);

	// spawn ghosts depending on ghostCnt
	std::thread *thread_ptr;
	std::thread *thread_ptr2;
	std::thread *thread_ptr3;

	thread_ptr = new thread(&Ghost1::spawn, ghost1);
	if(ghostCnt >= 2) {
		thread_ptr2 = new thread(&Ghost1::spawn, ghost2);
	}
	if (ghostCnt >= 3){ // max 3
		thread_ptr3 = new thread(&Ghost1::spawn, ghost3);
	}

	playGame(player);

	// join threads only if they were created
	thread_ptr->join();
	if(ghostCnt >= 2)
		thread_ptr2->join();
	if(ghostCnt >= 3)
		thread_ptr3->join();

	// delete
	delete thread_ptr;
	if(ghostCnt >= 2)
		delete thread_ptr2;
	if(ghostCnt >= 3)
		delete thread_ptr3;
}

int main(int argc, char** argv)
{
	// Setup
	setlocale(LC_ALL, "");
	WINDOW* win = initscr();
	defineColors();
	noecho(); // dont print anything to the screen


	for(CURRENT_LEVEL; CURRENT_LEVEL < 6; CURRENT_LEVEL++) {	
		string mapName = "map";
		mapName += ((char) '0' + CURRENT_LEVEL);
		mapName += ".txt";
		init(mapName.c_str(), 2, .75);
		if(GAME_WON == -1) {
			break;
		}
		else {
			GAME_WON = 0;
			TOTAL_POINTS = 0;
			THINKING = false;
		}
	}	
	/*
	clear();
	drawScreen("map1.txt");

	// Create Player
	avatar player(5,6, true);
//	player.moveRight();
	
	// Create ghost1
	Ghost1 ghost1(1, 1,.7, COLOR_BLUE);

	// Ghost 2
	Ghost1 ghost2(8, 4,.7, COLOR_RED);

	thread ghostThread1 (&Ghost1::spawn, ghost1);
	thread ghostThread2 (&Ghost1::spawn, ghost2);


	string key;
	while(key != "q" && GAME_WON == 0) {
		key = "";
		key += getch();
		if(key != "q")
			onKeystroke(player, key);
		stringstream ss;
		ss << "Points: " << player.getPoints() << "/" << TOTAL_POINTS << "\n";
		if(GAME_WON == 0)
			printAtBottom(ss.str());
		move(player.getY(), player.getX());
		refresh();
	}	
	GAME_WON = -1;
	printf("AHHH");
	
	//ghostThread1.join();
	//ghostThread2.join();

	clear();
	if(player.getPoints() >= TOTAL_POINTS) {
		winGame();
	}
	else { loseGame(); }
	*/
	//endwin();
	sleep(2);
	endwin();
	return 0;
}
