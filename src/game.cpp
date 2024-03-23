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

#include <vector>
#include <iostream>

#include "globals.h"
#include "helperFns.h"
#include "avatar.h"
#include "ghost1.h"

using namespace std;

// changed in DrawScreen and used when spawning the player
int START_X = 1;
int START_Y = 1;

// ghosts from text file
struct ghostInfo {
	double think;
	int xPos;
	int yPos;
};
vector<ghostInfo> ghostList;


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


// true if string only contains digits...regex would be nice
bool isFullDigits(string &str) {
	for(unsigned i = 0; i < str.size(); i++) {
		if(!isdigit(str[i]))
			return false;
	}
	return true;
}



void doKeystroke(avatar& unit) {
	if(INPUT== "q") { 
		endwin();
		exit(0);
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
	else if(INPUT.size() == 2 && (INPUT[0] == 'f')){
	  unit.jumpForward(INPUT[1], true);
	  INPUT="";
	}
	else if(INPUT.size() == 2 && (INPUT[0] == 'F')){
	  unit.jumpBackward(INPUT[1], true);
	  INPUT="";
	}
	else if(INPUT.size() == 2 && (INPUT[0] == 't')){
	  unit.jumpForward(INPUT[1], false);
	  INPUT="";
	}
	else if(INPUT.size() == 2 && (INPUT[0] == 'T')){
	  unit.jumpBackward(INPUT[1], false);
	  INPUT="";
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
			unit.parseToBeginning();
		}
		// move to the first word on the current line
		INPUT = "^";
		doKeystroke(unit);
	}
	else if(INPUT == "^") {
		// goes to first character after blank
		unit.parseToBeginning();

		char currentChar = charAt(unit.getX(), unit.getY());
		if (currentChar == ' ') {
			unit.parseWordForward(true);
		}
	}
	else if(INPUT == "&") {
		GAME_WON = 1; // l337 cheetz
	}
}	

void onKeystroke(avatar& unit, char key) {
	mtx.lock();
	writeError("ON KEY STROKE");
	writeError("CURRENT INPUT: " + INPUT + key);

	// there are some weird edge cases which I want to handle here:
	// 1. #G [moves to line #]
	// 2. 1G = same as gg
	// 3. gg = beginning of file... it's weird bc it's two non-digit characters
	// 4. fx = for any x character, jump forward to it, Fx for backward

	// if f/F/t/T was pressed, allow every character
	if(INPUT.size() == 1 && (INPUT[0] == 'f' || INPUT[0] == 'F'
	                      || INPUT[0] == 't' || INPUT[0] == 'T')) {
	  INPUT += key;
	}
	else if(key == 'g') { 
		// have 'g' (only) in buffer, or buffer is empty
		if(INPUT.empty() || (INPUT.size() == 1 && INPUT[0] == 'g')) {	
			
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
	// If INPUT != empty, and the user inputs a number, INPUT
	// should reset.. EG: 3g3 dd = 1 dd, not 3 dd
	else if(!INPUT.empty() && isdigit(key) && !isFullDigits(INPUT)) {
		// reset it.. can't enter a digit in the middle of input
		INPUT = "";
	}
	// we have full digits and then enter a character
	else if(!INPUT.empty() && isFullDigits(INPUT) && !isdigit(key)) { 
		int num = std::stoi(INPUT, nullptr, 0); // extracts 33 from 33dd for example
		
		// special ... #G. Move to the line number #
		if(key == 'G') {
			// go to line num
			if(num == 1) {
				INPUT = "1G";
				doKeystroke(unit);
			}
			// don't go to a line that is offscreen or off the map
			else if(num > TOP) {
				INPUT = "G";
				doKeystroke(unit);
			}
			else {
				// change line number
				unit.setPos(unit.getX(), num);
				INPUT = "";
				
				// then go to the first character
				mtx.unlock();
				onKeystroke(unit, '^');
			}
			refresh();
			mtx.unlock();
			return;
		}
		// if the input is NOT G, then it means
		// we are repeating a keystroke.. eg 3w = w, three times
	
		INPUT = key; 
		for(int i = 0; i < num; i++) {
			doKeystroke(unit);
		}
		INPUT = "";
	}
	else {
		// the first time we enter something
		INPUT += key;

		if(INPUT.size() == 2 && (INPUT[0] == 'f' || INPUT[0] == 'F'
		                      || INPUT[0] == 't' || INPUT[0] == 'T')){
			doKeystroke(unit);
			INPUT = "";
		}
		// do keystroke if the first character is a letter,
		//  except 0 (which immediately moves the player)
		if(INPUT == "0" || (!isFullDigits(INPUT) && INPUT != "f" && INPUT != "F" && INPUT != "t" && INPUT != "T")) {
			doKeystroke(unit);
			INPUT = "";
		}
	}
	refresh();
	mtx.unlock();
}

// called right before a level loads
void levelMessage() {
	// find appropriate message
	stringstream ss;
	ss << "LEVEL " << CURRENT_LEVEL;
	string msg = ss.str();

	// print + pause
	printw("%s", msg.c_str());
	refresh();
	usleep(1500000);

	// clear and reset everything
	clear();
	move(0,0);
	refresh();
}

// loads the level, essentially
void drawScreen(const char* file) {
	levelMessage();
	clear();
	
	writeError("DRAWING THE SCREEN");

	ifstream in(file);

	// clear ghostList because we are gonna obtain new ones
	ghostList.clear();

	vector<vector <chtype> > board;
	vector<string> boardStr;
	string str;
	vector<chtype> line;

	// store lines from text file into 'board' and 'boardStr'
	WIDTH = 0; // largest width in the map
	while(getline(in, str)) {
		for(unsigned i = 0; i < str.length(); i++) {
			line.push_back(str[i]);
		}
		boardStr.push_back(str);
		board.push_back(line);
		line.clear();

		if (WIDTH < str.length())
			WIDTH = str.length();
	}
	
	// add spaces automatically to lines that don't have
	// the max length (specified by WIDTH). Errors will
	// happen if the board does not have a constant length
	for(unsigned i = 0; i < board.size(); i++) {
		boardStr.at(i).resize(WIDTH, ' '); 
		for(unsigned j = board.at(i).size(); j < WIDTH; j++) { 
			chtype empty = ' ';
			board.at(i).push_back(empty);
		}
	}
	in.close();

	// iterate thru each line, parse, create board, create ghost attributes 
	for(unsigned i = 0; i < board.size(); i++) {

		// parse info about ghosts, add them to ghostlist
		if(boardStr.at(i).at(0) == '/') {
			// format: /*thinkTime* *x-position* *y-position* -- delimited by spaces ofc
			// EG: /1.5 19 7
			string str = boardStr.at(i);
			str.erase(str.begin(), str.begin()+1);

			string a = str.substr(0, str.find(" "));
			str = str.substr(str.find(" ")+1, 9); // 9 is kinda arbitrary (should be > str length)

			string b = str.substr(0, str.find(" "));
			str = str.substr(str.find(" ")+1, 9);

			string c = str.substr(0, str.find(" "));
			str = str.substr(str.find(" ")+1, 9);
		
			// create the ghost
			ghostInfo ghost;
			ghost.think = stod(a, nullptr);
			ghost.xPos = stoi(b, nullptr, 0);
			ghost.yPos = stoi(c, nullptr, 0);
			ghostList.push_back(ghost);
			continue;
		}
		// this is where the player starting position is handled 
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

			return; // player position should always be the last thing in a file
		}
		// this is where we actually draw the board
		for(unsigned j = 0; j < board.at(i).size(); j++) {

			// TOTAL_POINTS is incremented by 1 if a letter is found;
			// it represents the number of letters the player has to step on to win
			if(board.at(i).at(j) != '~' && 
				board.at(i).at(j) != ' ' &&  board.at(i).at(j) != '#') 
				TOTAL_POINTS++;


			// Check for walls -- the wall character depends on the position
			// of the other walls. EG: is the wall a corner, a straight line, etc?
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
			// Check down
			if((i+2) < (board.size())) {
				if(board.at(i+1).at(j) == '#') {
					down = true;
				}
			}
                                
			// add the appropriate wall 
			if(*ch == '#') {
				attron(COLOR_PAIR(3)); // yellow, but can change
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
				// special color for tilde keys
				if(*ch == '~') {
					attron(COLOR_PAIR(6));
				}
				addch(*ch);
				attroff(COLOR_PAIR(6));
			}
		}
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
		TOP++;
		writeError("TOP is set");	
		addch('\n');
	}

	
	// if the 'p' in a file is not found, that means no player starting
	// position was specified, and therefore we set the default here:
	START_X = WIDTH/2;
	START_Y = HEIGHT/2;
}



void defineColors() {
	start_color();
	init_pair(1, COLOR_RED	, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE	, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
}


void playGame(time_t lastTime, avatar &player) {

	// consume any inputs in the buffer, or else the inputs will affect
	// the game right as it begins by moving the player 
	char ch;
	usleep(10000);
	printAtBottom("PRESS ENTER TO PLAY!\n    ESC OR q TO EXIT!");
	while(true) {
		
		ch = getch();
		
		if(ch == '\n') {
			if(time(0) > (lastTime)) {
				READY = true;
				break;
			}
		}
		// quit the game if we type escape or q
		else if(ch == 27 || ch == 'q'){
				endwin();
				exit(0);
		}
	}
	printAtBottom("GO!                  \n                       ");
	char key;
	
	// continue playing until the player hits q or the game is over
	while(GAME_WON == 0) {
		key = getch();

		onKeystroke(player, key);
		stringstream ss;

		// increment points as game progresses
		ss << "Points: " << player.getPoints() << "/" 
			<< TOTAL_POINTS << "\n" << " Lives: " << LIVES << "\n";
		if(GAME_WON == 0)
			printAtBottom(ss.str());

		// redundant movement
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


void init(const char* mapName) {
	// set up map
	clear();
	TOP = 0;
	BOTTOM = 0;
	WIDTH = 0;
	drawScreen(mapName);

	// create player
	avatar player (START_X, START_Y, true);

	// spawn ghosts	
	std::vector<std::thread> ghost_threads;
	for(int i = 0; i < ghostList.size(); ++i){
		Ghost1 ghost = Ghost1(ghostList[i].xPos, ghostList[i].yPos,
			(THINK_MULTIPLIER * ghostList[i].think), COLOR_RED);

		ghost_threads.push_back(thread(&Ghost1::spawnGhost, ghost, false));
	}
	
	// begin game	
	playGame(time(0), player);
	writeError("GAME ENDED!");

	// join threads
	for(auto& ghost_thread : ghost_threads){
		ghost_thread.join();
	}
}

bool checkParams(int argc, char** argv) {
	//returns true if success, false if there is any error
	std::vector<string> params; // command params except first one (where the program is called)
	for (int i = 0; i < argc - 1; ++i)
	{
		params.push_back(argv[i+1]);
	}
	
	// yes, I know that you could optimize by doing it in one cycle but:
	// 1) it is not noticable
	// 2) I think this approach is more readable and allows further use of the "sanitized" input
	for (int i = 0; i < params.size(); ++i)
	{
		string currentParam = params[i];

		if (isFullDigits(currentParam)) // level select
		{
			int new_level = std::stoi(currentParam, nullptr, 0);
			if (new_level > NUM_OF_LEVELS || new_level < 0) {
				endwin();
				cout << "\nInvalid starting level." << endl << endl;
				return false;
			}
			CURRENT_LEVEL = new_level;
		}
		else if ( currentParam.length() == 1 ) // check for hard/normal mode
		{
			char mode = currentParam[0]; // h=hard, n=normal
			if (mode == 'h')
			{
				// hard/default mode
				THINK_MULTIPLIER = 1.0;
			}
			else if (mode == 'n')
			{
				// normal mode
				THINK_MULTIPLIER = 1.2; // 20% slower ghosts
			}
			else
			{
				endwin();
				cout << "\nInvalid mode argument, only h/n allowed. Example: ./pacvim n" << endl << endl;
				return false;
			}
		}
		else
		{
			endwin();
			cout << "\nInvalid arguments. Try ./pacvim or ./pacvim [#] [h/n]" <<
				"\nEG: ./pacvim 8 n" << endl << endl;
			return false;
		}
	}

	return true;
}

int main(int argc, char** argv)
{
	// Setup
	WINDOW* win = initscr();
	defineColors();
	noecho(); // dont print anything to the screen

	// Look for cmd line args
	// Any cmd line args will change the CURRENT_LEVEL
	// at the start of the game.
	// EG: ./pacvim 4 --> player starts on 4th level

	if( ! checkParams(argc, argv) )
	{
		// program called with invalid arguments
		return 0;
	}

	while(LIVES >= 0) {
		string mapName = MAPS_LOCATION "/map";
		
		// convert CURRENT_LEVEL to string, and load
		std::stringstream ss;
		ss << CURRENT_LEVEL;
		
		mapName += ss.str(); // add it to mapName
		mapName += ".txt"; // must be .txt
		init(mapName.c_str());
		if(GAME_WON == -1) {
			CURRENT_LEVEL--;
			GAME_WON = 0;
			TOTAL_POINTS = 0;
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
		}
		CURRENT_LEVEL++;
		// Start from beginning now
		if(CURRENT_LEVEL > NUM_OF_LEVELS) {
			CURRENT_LEVEL = 0;
			THINK_MULTIPLIER *= 0.8;
		}
	}	
	//endwin();
	sleep(2);
	endwin();
	return 0;
}          
