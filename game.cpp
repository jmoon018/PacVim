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
		gotoLine(unit,1);
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
	printAtBottom("getting keystroke..");	
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
		//writeError("received: g");
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
	clear();
	string msg = "LEVEL " + ('0' + CURRENT_LEVEL);
	printAtBottom(msg);
	sleep(1);
	printAtBottom("");
	usleep(500000); // .5 seconds
	printAtBottom(msg);
	sleep(1);
	printAtBottom("GOOD LUCK!");
	sleep(1);
}

void drawScreen(const char* file) {
	levelMessage();
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

			if(maze.at(i).at(j) != ' ' &&  maze.at(i).at(j) != '#') 
				TOTAL_POINTS++;

			if(maze.at(i).at(j) == '#')
				attron(COLOR_PAIR(3));
			else if(maze.at(i).at(j) == '~') {
				attron(COLOR_PAIR(5));
				TOTAL_POINTS--;
			}
			addch(maze.at(i).at(j));
			attroff(COLOR_PAIR(5) | COLOR_PAIR(3));
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

	HEIGHT= fixedMaze.size();
	WIDTH = fixedMaze.at(0).size();
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



void f1(int x) {
	cout << "X" << flush;
	usleep(50 * 1000);
	if(x > 0)
		f1(x-1);
}

void f2(int x) {
	cout << "Y" << flush;
	usleep(50 * 1000);
	if(x > 0)
		f2(x-1);
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


void fn1() {
	/*
	for(int i = 0; i < 500; i++) { 
	//	mtx.lock();
		x += charAt(i%6, i%3) ;		
		writeAt(i%15, (3*i)%15, 'X'); 
		refresh();
	//	mtx.unlock();
	}
	*/
	for(int j = 0; j < 2225; j++) {
	for(int i = 0; i < 26; i++) {
		chtype letter = charAt(i, 1);
		writeAt(i, 0, letter);
	}
	}
//	mtx.unlock();
}

void fn2() {
	/*
	for(int j = 0; j < 500; j++) { 
	//	mtx.lock();
		x += charAt(j%5, j%9);
		writeAt(j%5, (3*j)%5, 'Y'); 
		refresh();
	//	mtx.unlock();
	}
	*/

	for(int j = 0; j < 2225; j++ )  {
	for(int i = 0; i < 26; i++ ) {
		chtype letter = charAt(i, 1);
		writeAt(i, 0, letter);
	}
	}
	//mtx.unlock();
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
	// set up map
	clear();
	drawScreen(mapName);

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
	delete thread_ptr2;
	delete thread_ptr3;
}

int main(int argc, char** argv)
{
	// Setup
	setlocale(LC_ALL, "");
	WINDOW* win = initscr();
	defineColors();
	noecho(); // dont print anything to the screen


	for(CURRENT_LEVEL; CURRENT_LEVEL < 3; CURRENT_LEVEL++) {	
		string mapName = "map";
		mapName += ((char) '0' + CURRENT_LEVEL);
		mapName += ".txt";
		init(mapName.c_str(), 3, .75);
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
