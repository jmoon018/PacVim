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

// more globals cause i'm lazy
int START_X = 1;
int START_Y = 1;

// ghosts
struct ghostInfo {
	double think;
	int xPos;
	int yPos;
};

vector<ghostInfo> ghostList;

void gotoLine(avatar& unit, int line) {
	int wallCnt = 2;
	char curChar = mvinch(line, 0);
	while(wallCnt > 0) {
		}
}

void gotoLineBeginning(int line, avatar &unit) {
	int x = 0;
	while(mvinch(x, line) == '#' ) {
		x++;
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
		//clear();
		refresh();	
	}
	else if(INPUT == "w") { unit.parseWordForward(true); 
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
	else if(INPUT == "gg" || INPUT == "1G") {
		int i = 0;
		while(!isInside(unit.getX(), BOTTOM+i, "omni")) {
			i++;
			unit.setPos(0, BOTTOM+i);
			unit.parseToBeginning();
		}
		unit.setPos(unit.getX(), BOTTOM+i); 
		unit.parseToBeginning();
	}
	else if(INPUT == "G") { 
		int i = 0;
		while(!isInside(unit.getX(), TOP-i, "omni")) {
			i++;
			unit.setPos(unit.getX(), TOP-i);
			unit.parseToEnd();
		}
		unit.parseToEnd();
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
	mtx.lock();
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
			if(INPUT == "gg") {
				doKeystroke(unit);
				INPUT = "";
			}
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
			if(num == 1) {
				INPUT = "1G";
				doKeystroke(unit);
			}
			else if(num > TOP) {
				INPUT = "G";
				doKeystroke(unit);
			}
			else {
				unit.setPos(unit.getX(), num);
				INPUT = "^";
				doKeystroke(unit);
			}
			refresh();
			THINKING = false;
			mtx.unlock();
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
	mtx.unlock();
}

void levelMessage() {
	// find appropriate message
	stringstream ss;

	ss << "LEVEL " << CURRENT_LEVEL;
	string msg = ss.str();

	printw(msg.c_str());
	refresh();
	usleep(1500000);

	// clear and reset everything
	clear();
	move(0,0);
	refresh();
}

void drawScreen(const char* file) {
	levelMessage();
	clear();

	ifstream in(file);

	// clear ghostList because we are gonna obtain new ones
	ghostList.clear();

	vector<vector <chtype> > board;
	vector<string> boardStr;
	string str;
	vector<chtype> line;

	while(getline(in, str)) {
		for(unsigned i = 0; i < str.length(); i++) {
			line.push_back(str[i]);
		}
		boardStr.push_back(str);
		board.push_back(line);
		line.clear();
		writeError(str);
	}
	in.close();
	for(unsigned i = 0; i < board.size(); i++) {
		unsigned length = board.size();
		stringstream ss;
		ss << "On row.." << i << "..." << boardStr.at(i);
		writeError(ss.str());
		// parse info about ghosts, add them to ghostlist
		if(boardStr.at(i).at(0) == '/') {
			// format: /*thinkTime* *x-position* *y-position* -- delimited by spaces ofc
			string str = boardStr.at(i);
			str.erase(str.begin(), str.begin()+1);

			string a = str.substr(0, str.find(" "));
			str = str.substr(str.find(" ")+1, 9); // 9 is kinda arbitrary (should be > str length)

			string b = str.substr(0, str.find(" "));
			str = str.substr(str.find(" ")+1, 9);

			string c = str.substr(0, str.find(" "));
			str = str.substr(str.find(" ")+1, 9);
		
			writeError(a);
			writeError(b);
			writeError(c);
			ghostInfo ghost;
			ghost.think = stod(a, nullptr);
			ghost.xPos = stoi(b, nullptr, 0);
			ghost.yPos = stoi(c, nullptr, 0);
			ghostList.push_back(ghost);
			writeError("xx");
			continue;
		}
	    else if(boardStr.at(i).at(0) == 'p') {
			string str = boardStr.at(i);
		    str.erase(str.begin(), str.begin()+1); 

			// get x position
			string x = str.substr(0, str.find(" "));
			str = str.substr(str.find(" ")+1, 9); // delete up to space

			string y = str.substr(0, str.find(" "));
			str = str.substr(str.find(" ")+1, 9); // delete up to space

			START_X = stoi(x, nullptr, 0);
			START_Y = stoi(y, nullptr, 0);

			return; // player position should always be the last thing
		}
		for(unsigned j = 0; j < board.at(i).size(); j++) {
			stringstream ss;
			ss << board.at(i).at(j) << "..." << j;
			writeError(ss.str());
			if(board.at(i).at(j) != '~' && 
				board.at(i).at(j) != ' ' &&  board.at(i).at(j) != '#') 
				TOTAL_POINTS++;

			bool left = false, right = false,
				up = false, down = false;
			chtype* ch = &( board.at(i).at(j));
			// Check left
			if(j >= 1) {
				if(board.at(i).at(j-1) == '#') {
					left = true;
				}
			}
			// Check right
			if((j+1) < (board.at(i).size())) {
				if(board.at(i).at(j + 1) == '#') {
					right = true;
				}
			}
			// Check up
			if(i >= 1) {
				if(board.at(i - 1).at(j) == '#') {
					up = true;
				}
			}
			writeError("Up works");
			// Check down
			if((i+2) < (board.size())) {
				if(board.at(i+1).at(j) == '#') {
					down = true;
				}
			}
			writeError("Down works.");
                                	
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
		}
		if(board.at(i).size() > WIDTH) {
			WIDTH = board.at(i).size();
		}
		writeError("eek");
		
		// set value of BOTTOM - which is the first row
		//	in which a player can move in
		int size = board.at(i).size();
		if(i != 0 && BOTTOM == 0) {	
			bool INSIDE = false;
			char lastChar;
			for(int j = 0; j < size; j++) { 
				if(board.at(i).at(j) == '#') {
					if(lastChar != '#')
						INSIDE = !INSIDE; // true -> false, false -> true
				}
				else {
					BOTTOM = i;
					break;
				}
				INSIDE = false;
				lastChar = board.at(i).at(j);
			}
		}
		// increment height only if there are valid spaces
		// for the player to land on (in otherwords, they aren't all hashtags)
		//for(int j = 0; j < size; j++) { 
			//if(boardStr.at(i).at(j) != '#') {
				TOP++;
				//break;
			//}
		//}
		writeError("Height is set");	
		addch('\n');
	}
	START_X = WIDTH/2;
	START_Y = HEIGHT/2;
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


void playGame(time_t lastTime, avatar &player) {
	//sleep(1);
	usleep(10000);
	printAtBottom("PRESS ENTER TO PLAY!");
	while(true) {
		if(getch() == '\n') {
			if(time(0) > (lastTime)) {
				READY = true;
				break;
			}
		}
	}
	printAtBottom("GO!                  ");
	char key;
	while(key != 'q' && GAME_WON == 0) {
		key = getch();
		if(key != 'q')
			onKeystroke(player, key);
		stringstream ss;
		ss << "Points: " << player.getPoints() << "/" 
			<< TOTAL_POINTS << "\n" << " Lives: " << LIVES << "\n";
		if(GAME_WON == 0)
			printAtBottom(ss.str());
		move(player.getY(), player.getX());
		refresh();
	}	
	
	clear();
	if(GAME_WON == 1) {
		winGame();
	}
	else {
		loseGame();
	}
}


void init(const char* mapName, int ghostCnt, double thinkMultiplier) {
	// set up map
	clear();
	TOP = 0;
	BOTTOM = 0;
	WIDTH = 0;
	drawScreen(mapName);

	// create player
	avatar player (START_X, START_Y, true);
	

	// pointers to the ghost threads
	std::thread *thread_ptr;
	std::thread *thread_ptr2;
	std::thread *thread_ptr3;
	std::thread *thread_ptr4;
	std::thread *thread_ptr5;
	
	
	// Create ghosts if they exist in text file
	// we can spawn 5 total at the moment, but more can be manually added below
	ghostCnt = ghostList.size();
	Ghost1 ghost1, ghost2, ghost3, ghost4, ghost5; 
	ghost1 = Ghost1(ghostList.at(0).xPos, ghostList.at(0).yPos, 
			(THINK_MULTIPLIER * ghostList.at(0).think), COLOR_RED); 
	if(ghostCnt >= 2) {	
		ghost2 = Ghost1(ghostList.at(1).xPos, ghostList.at(1).yPos, 
			(THINK_MULTIPLIER * ghostList.at(1).think), COLOR_RED); 
	}
	if(ghostCnt >= 3) {	
		ghost3 = Ghost1(ghostList.at(2).xPos, ghostList.at(2).yPos, 
			(THINK_MULTIPLIER * ghostList.at(2).think), COLOR_RED); 
	}
	if(ghostCnt >= 4) { 
		ghost4 = Ghost1(ghostList.at(3).xPos, ghostList.at(3).yPos, 
			THINK_MULTIPLIER * ghostList.at(3).think, COLOR_RED); 
	}
	if(ghostCnt >= 5) { 
		ghost5 = Ghost1(ghostList.at(4).xPos, ghostList.at(4).yPos,
			THINK_MULTIPLIER * ghostList.at(4).think, COLOR_RED);
	}
		
	// spawn ghosts
	thread_ptr = new thread(&Ghost1::spawnGhost, ghost1, false);
	if(ghostCnt >= 2) {
		thread_ptr2 = new thread(&Ghost1::spawnGhost, ghost2, false);
	}
	if (ghostCnt >= 3){ // max 3
		thread_ptr3 = new thread(&Ghost1::spawnGhost, ghost3, false);
	}
	if (ghostCnt >= 4){ // max 4
		thread_ptr4 = new thread(&Ghost1::spawnGhost, ghost4, false);
	}
	if (ghostCnt >= 5){ // max 5
		thread_ptr5 = new thread(&Ghost1::spawnGhost, ghost5, false);
	}
	
	
	// begin game	
	playGame(time(0), player);
	writeError("END DAMNIT!");

	// join threads only if they were created
	thread_ptr->join();
	if(ghostCnt >= 2)
		thread_ptr2->join();
	if(ghostCnt >= 3)
		thread_ptr3->join();
	if(ghostCnt >= 4)
		thread_ptr4->join();
	if(ghostCnt >= 5)
		thread_ptr5->join();

	// delete
	delete thread_ptr;
	if(ghostCnt >= 2)
		delete thread_ptr2;
	if(ghostCnt >= 3)
		delete thread_ptr3;
	if(ghostCnt >= 4)
		delete thread_ptr4;
	if(ghostCnt >= 5)
		delete thread_ptr5;
}

int main(int argc, char** argv)
{
	// Setup
	setlocale(LC_ALL, "");
	WINDOW* win = initscr();
	defineColors();
	noecho(); // dont print anything to the screen


	while(LIVES >= 0) {
		string mapName = "maps/map";
		mapName += ((char) '0' + CURRENT_LEVEL);
		mapName += ".txt";
		init(mapName.c_str(), 2, .75);
		if(GAME_WON == -1) {
			CURRENT_LEVEL--;
			GAME_WON = 0;
			TOTAL_POINTS = 0;
			THINKING = false;
		}
		else {
			if(GAME_WON == -1) {
				CURRENT_LEVEL--; // lost the game, repeat the level
			}
			else if ((CURRENT_LEVEL % 3) == 0) {
				LIVES++; // gain a life every 3 levels
			}
				
			GAME_WON = 0;
			TOTAL_POINTS = 0;
			THINKING = false;
		}
		CURRENT_LEVEL++;
		// Start from beginning now
		if(CURRENT_LEVEL == 10) {
			CURRENT_LEVEL = 0;
			THINK_MULTIPLIER *= 0.8;
		}
	}	
	//endwin();
	sleep(2);
	endwin();
	return 0;
}
