#include <thread>
#include <math.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <cursesw.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <list>
#include <set>
using namespace std;

// GLOBAL VARIABLES+CONSTANTS
int TOTAL_POINTS= 0;
int GAME_WON = 0; // -1 is a loss, 1 is win, 0 means game in progress

// THINKING
bool THINKING = false;

// DIMENSIONS
int HEIGHT;
int WIDTH;

int wallist[] = {'#', ACS_ULCORNER, ACS_LLCORNER, ACS_URCORNER,
				ACS_LRCORNER, ACS_LTEE, ACS_RTEE, ACS_BTEE,
				ACS_TTEE, ACS_HLINE, ACS_VLINE, ACS_PLUS};

set<int> WALLS(wallist, wallist + sizeof(wallist)/sizeof(int));


/* HELPER FUNCTIONS */

char charAt(int x, int y) {
	if(x < 0 || y < 0)
		return 0;

	int curX, curY;
	getyx(stdscr, curY, curX);

	char value = mvinch(y, x);
	mvinch(curY, curX);

	return value;
}

bool writeAt(int x, int y, char letter, int color = COLOR_WHITE) {
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
	string x = ""; x += msg;
	mvprintw(20, 1, x.c_str());
}
void printAtBottom(string msg) {
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




// Avatar Class -- can be a ghost or player
class avatar {
	public:
		avatar();
		avatar(int, int);
		avatar(int, int, bool);
		avatar(int, int, bool, int);
	protected:
		char letterUnder;
		int x;
		int y;
		bool isPlayer;
		int points;
		char portrait;
		int lives;
		int color;
	public:	
		bool moveTo(int, int);
		bool moveTo(int, int, bool);
		bool moveRight();
		bool moveLeft();
		bool moveUp();
		bool moveDown();
		void parseWordForward(bool);
		void parseWordBackward(bool);
		void parseWordEnd(bool);

		int getPoints();
		bool getPlayer();
		int getX();
		int getY();
		bool setPos(int, int);
		char getPortrait();

		void setLetterUnder(char);
};
	
avatar::avatar() {
	x = 1;
	y = 1;
	lives = 3;
	points = 0;
	portrait = 'G';
	isPlayer = false;
	color = COLOR_WHITE;
}

avatar::avatar(int a, int b) {
	x = a;
	y = b;
	lives = 3;
	points = 0;
	portrait = 'G';
	isPlayer = false;
	color = COLOR_WHITE;
}


avatar::avatar(int a, int b, bool human) {
	x = a;
	y = b;
	lives = 3;
	points = 0;
	isPlayer = human;
	if(human)
		portrait = ' '; // default for player
	else
		portrait = 'G';
}

avatar::avatar(int a, int b, bool human, int c) {
	avatar(a, b, human);
	color = c;
}

int avatar::getPoints() { return points; }
bool avatar::getPlayer() { return isPlayer; }
int avatar::getX() { return x; }
int avatar::getY() { return y; }
char avatar::getPortrait() { return portrait; }

bool avatar::setPos(int x, int y) { 
	if(!isValid(x, y))
		return false;
	moveTo(x, y);
	return true;
}


bool avatar::moveTo(int a, int b) {
	// Is it a valid spot?
	if(!isValid(a, b)) 
		return false;

	// Update stats
	x = a;
	y = b;

	char curChar = charAt(a, b); 
	letterUnder = curChar;
	writeAt(a, b, portrait, color);

	if(isPlayer) {
		// Check if Player hit a ghost
		if(charAt(a, b) == 'G') {
			loseGame();
			return false;
		}
		move(b, a);
	}
	else {
		// check if ghost hit a player
		int playerX, playerY;
		getyx(stdscr, playerY, playerX);
		if(playerY == b && playerX == a) {
			loseGame();
			return false;
		}
	}
		

	if(curChar != ' ')
		points++;
	
	if(points >= TOTAL_POINTS) {
		winGame();
	}
	printAtBottom("moving player");
	refresh();
	return true;
}

bool avatar::moveTo(int a, int b, bool del) {
	if(!isValid(a,b))
		return false;

	if(isPlayer) {
		// Check if the player hit a ghost
		if(charAt(a, b) == 'G') {
			loseGame();
			return false;
		}

		move(b, a);
		x = a;
		y = b;
		refresh();
		return true;
	}
	else {	
		// Check if Ghost hit the player
		int playerX, playerY;
		getyx(stdscr, playerY, playerX);
		if(playerY == b && playerX == a) {
			loseGame();
			return false;
		}
	}

	char curChar = charAt(a, b);
	if(del)
	{
		writeAt(a, b, portrait, color);
		if(curChar != ' ')
			points++;
		if(points >= TOTAL_POINTS) {
			winGame();	
		}
	}
	else {
		writeAt(x, y, letterUnder, COLOR_WHITE);
		writeAt(a, b, portrait, color);
	}
	x = a;
	y = b;

	letterUnder = curChar;
	
	refresh();
	return true;
}

bool avatar::moveRight() {
	if(!isValid(x+1, y)) 
		return false;
	
	moveTo(x+1, y);
	return true;
}

bool avatar::moveLeft() {
	if(!isValid(x-1, y))
		return false;
	
	moveTo(x-1, y);
	return true;
}

bool avatar::moveUp() {
	if(!isValid(x,y-1)) 
		return false;
	
	moveTo(x, y-1);
	return true;
}

bool avatar::moveDown() {
	if(!isValid(x,y+1))
		return false;
	moveTo(x, y+1);
	return true;
}

void avatar::parseWordEnd(bool isWord) {
	// Formula: Get next char, is it alphanumeric? If so, loop & break
	//		on nonalpha-, and viceversa. 
	// 2nd case: if you are not at the end of a word, loop until you
	//		reach a space

	// store the current character type
	char curChar = charAt(x, y);
	bool isAlpha = isalnum(curChar);
	char nextChar = charAt(x+1,y);

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
			if(!moveTo(x+1, y, false))
				break;
			if(nextChar != ' ')
				breakOnSpace = true;
			nextChar = charAt(x+1, y);
		}
	}
}
				
void avatar::parseWordBackward(bool isWord) {
	// Formula: Get next char, is it alphanumeric? If so, loop & break
	//		on nonalpha-, and viceversa. 
	// 2nd case: if you are not at the end of a word, loop until you
	//		reach a space

	// store the current character type
	char curChar = charAt(x, y); 
	bool isAlpha = isalnum(curChar);
	char nextChar = charAt(x-1, y); 

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

			if(!moveTo(x-1, y, false))
				break;
			if(nextChar != ' ')
				breakOnSpace = true;
			nextChar = charAt(x-1, y); 
		}
	}
}

void avatar::parseWordForward(bool isWord) {
	char curChar = charAt(x, y); 
	bool isAlpha = isalnum(curChar);
	char lastChar= 'X';

	bool breakOnAlpha = !isalnum(curChar);
	
	while(true) {
		if((!isalnum(curChar) == !breakOnAlpha) && isWord ) { // if they are the same
			break;
		}
		else if(lastChar == ' ' && curChar != ' ') {
			break;
		}
		else if(lastChar == '#' || curChar == '#') {
			moveTo(x-1, y, false);
			break;
		}
		else {
			lastChar = curChar;
			if(!moveTo(x+1,y,false))
				break;
			curChar = charAt(x,y);
		}
	}
}



// GHOST

class Ghost1 : public avatar {
	private:
		double sleepTime;
		double eval();
		double eval(int a, int b);
		void think();
	public:
		bool spawn();
		Ghost1(int a, int b, double c) : avatar(a, b) { sleepTime = c; }
		Ghost1(int a, int b) : avatar(a, b) { sleepTime = 0.5; }
		Ghost1() : avatar() { sleepTime = 0.5; }
		Ghost1(double time) : avatar() { sleepTime = time; }
		Ghost1(int a, int b, double c, int col) : avatar(a, b) { sleepTime = c; color = col; }
};

double Ghost1::eval() {
	// Determine how far ghost is away from player
	int playerX, playerY;
	getyx(stdscr, playerY, playerX);

	return sqrt( pow(playerY-y, 2.0) + pow(playerX - x, 2.0) );
}

double Ghost1::eval(int a, int b) {
	if(!isValid(a,b))
		return 1000;
	int playerX, playerY;
	getyx(stdscr, playerY, playerX);

	return sqrt(pow(playerY-b, 2.0) + pow(playerX-a, 2.0));
}

/*
struct node {
	int x;
	int y;
	bool dest;
	int dist = 10000;
};

const int HEIGHT2 = 5;
const int WIDTH2 = 5;
node GRAPH[HEIGHT2][WIDTH2] ;

void initGraph() {
	
	for(int i = 0; i < HEIGHT; i++) {
		for(int x = 0; x < WIDTH; x++) {
			node Node;
			Node.x = i;
			Node.y = x;
			Node.dest = false;
			GRAPH[i][x] = Node;
		}
	}
}


double Ghost1::AStar() {
	// get the destination
	int playerX, playerY;
	getyx(stdscr, playerY, playerX);

	vector<int> dist;
	vector<node> unvisited; 

	initGraph();
	GRAPH[playerX, playerY].dest = true;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			unvisited.push_back(GRAPH[i][j]);
			dist.push_back(10000);
		}
	}

	node nextNode = GRAPH[x][y];
	while(!unvisited.empty()) {
		node n = nextNode; 
		
		// check up
}
*/


void Ghost1::think() {
	if(THINKING)
		usleep(sleepTime * 1000000 * 0.5);

	THINKING = true;
	//cout << "Thinking.." << endl;
	// evaluate the four potential paths and move accordingly
	double up = eval(x, y-1);
	double down = eval(x, y+1);
	double left = eval(x-1, y);
	double right = eval(x+1, y);

	if(up <= down && up <= left && up <= right) 
		moveTo(x, y-1, false);
	else if(down <= left && down <= right && down <= up) 
		moveTo(x, y+1, false);
	else if(left <= right && left <= up && left <= down) 
		moveTo(x-1, y, false);
	else if(right <= up && right <= down && right <= left)
		moveTo(x+1, y, false);	

	THINKING = false;
	usleep(sleepTime * 1000000);
	if(GAME_WON == 0)
		think();
}
	
bool Ghost1::spawn() {
	letterUnder = charAt(x, y);
	if(!moveTo(x, y))
		return false;
	
	sleep(1); // wait a second to create map, etc
	think();

	return true;
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
	printAtBottom("getting keystroke..");	
	if(key == "q") {
		endwin();
	}
	else if(key == "h") {
		unit.moveLeft();
	}
	else if(key == "j") {
		unit.moveDown();
	}
	else if(key == "k") {
		unit.moveUp(); 
	}
	else if(key == "l") {
		unit.moveRight();
	}
	else if(key == "r") {
		refresh();	
	}
	else if(key == "g") {
		getMore(unit, key);
	}
	else if(key == "w") {
		unit.parseWordForward(true); 
	}
	else if(key == "W") {
		unit.parseWordForward(false);	
	}
	else if(key == "b") {
		unit.parseWordBackward(true);
	}
	else if(key == "B") {
		unit.parseWordBackward(false);
	}
	else if(key == "E") {
		unit.parseWordEnd(false);
	}
	else if(key == "e") {
		unit.parseWordEnd(true);
	}
	else if(key == "gg") {
		gotoLine(unit,1);
	}

	key = "";
	refresh();
}

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
			if(maze.at(i).at(j) != ' ' &&  maze.at(i).at(j) != '#')
				TOTAL_POINTS++;
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
	init_pair(1, COLOR_RED	, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE	, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);

}

int main(int argc, char** argv)
{
	// Setup
	setlocale(LC_ALL, "");
	WINDOW* win = initscr();
	defineColors();
	noecho(); // dont print anything to the screen
	clear();
	drawScreen("map1.txt");

	// Create Player
	avatar player(5, 5, true);
	player.moveRight(); 


	// Create ghost1
	Ghost1 ghost1(1, 1, .25, COLOR_BLUE);
	//ghost1.spawn();
	thread ghostThread (&Ghost1::spawn, ghost1);

	Ghost1 ghost2(1, 1, .5, COLOR_RED);
	thread ghostThread2 (&Ghost1::spawn, ghost2);


	printAtBottom("Spawned ghost");
	while(GAME_WON == 0) {
		string key;
		printAtBottom("Trying to get key");
		key += getch();
		printAtBottom("Got the key..");
		if(key != "")
			onKeystroke(player, key);
		stringstream ss;
		ss << "Points: " << player.getPoints() << "/" << TOTAL_POINTS << "\n";
		printAtBottom(ss.str());
		move(player.getY(), player.getX());
		refresh();
	}	

	printf("GG"); refresh();
	sleep(1);
	endwin();
	return 0;
}
