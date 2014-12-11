#include <thread>
#include <string>
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

#include <ostream>
#include <fstream>
#include <mutex>
#include <chrono>
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


#include "helperFns.h"
// avatar
#include "avatar.h"

mutex mtx;

/* ERROR WRITING */
void writeError(string msg) {
//	cout << "CALLING WRITEERROR";
	ofstream fs;
	fs.open ("errors.log", fstream::app);
	fs << msg;
	fs << "\n";
	fs << "LOL";
	fs.close();
}


// GHOST

class Ghost1 : public avatar {
	private:
		double sleepTime;
		double eval();
		double eval(int a, int b);
	public:
		void spawn();
		Ghost1(int a, int b, double c) : avatar(a, b) { sleepTime = c; }
		Ghost1(int a, int b) : avatar(a, b) { sleepTime = 0.5; }
		Ghost1() : avatar() { sleepTime = 0.5; }
		Ghost1(double time) : avatar() { sleepTime = time; }
		Ghost1(int a, int b, double c, int col) : avatar(a, b) { sleepTime = c; color = col; }
		void backtrack(int &a, int &b);
		void think();
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
};:24


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

struct node { int x; int y; int val = 0; };

/*
bool hasNode(const set<node> &v, int a, int b) {
	for(int i = 0; i < v.size(); i++) {
		if(v.at(i).x == a && v.at(i).y == b) {
			return true;
		}
	}
	return false;
}
*/

/*
bool delNode(set<node> &v, int a, int b) {
	for(int i = 0; i < v.size(); i++) {
		if(v.at(i).x == a && v.at(i).y == b) {
			v.erase(v.begin() + i);
			return true;
		}
	}
	return false;
}
*/

bool hasNode(set<node> &s, int a, int b) {
	for(set<node>::iterator it = s.begin(); it!=s.end(); it++) {
		if(it->x == a && it->y == b) {
			return true;
		}
	}
	return false;
}

void Ghost1::backtrack(int &a, int &b) {
	set<node> checked;
	set<node> closed;

	node xNode;
	//checked.find(xNode);	
	// starting position
	int startX = x, startY = y;
	// Get destination (player)
	int dX, dY;
	getyx(stdscr, dY, dX);
	node curNode;
	curNode.x = x;
	curNode.y = y;
	int count = 0;
	
	string print;
	
	while(curNode.x != dX || curNode.y != dY) {
		//printAtBottom("searching...");
		//cout << count << endl;
		// tryup
		if(isValid(x, y-1) && !hasNode(checked, x, y-1) && !hasNode(closed, x, y-1)) { 
			y--;
		}
		else if(isValid(x+1, y) && !hasNode(checked, x+1, y) && !hasNode(closed, x+1, y)) { //try right
			x++;
		}
		else if(isValid(x-1, y) && !hasNode(checked, x-1, y+1) && !hasNode(closed, x-1, y)) { // try left
			x--;
		}
		else if(isValid(x, y+1) && !hasNode(checked, x, y+1) && !hasNode(closed, x, y+1)) { // try down
			y++;
		}
		else 
		{
			// OK looks like there are no open nodes adjacent to this location
			if(isValid(x, y-1) && !hasNode(closed, x, y-1)) { 
				y--;
			}
			else if(isValid(x+1, y) && !hasNode(closed, x+1, y)) { //try right
				x++;
			}
			else if(isValid(x-1, y) && !hasNode(closed, x-1, y)) { // try left
				x--;
			}
			else if(isValid(x, y+1) && !hasNode(closed, x, y+1)) { // try down
				y++;
			}	
			else { printAtBottom("CAN'T GO ANYWHERE LOL"); break; } 

			if(count == 0) {
				a = x;
				b = y;
			}
			count++;
			//if(delNode(checked, curNode.x, curNode.y)) { printAtBottom("DAMN"); }
			set<node>::iterator it;
			//bool exists = checked.find(curNode) != checked.end();
			//checked.erase(checked.find(curNode));
//			closed.insert(curNode);
			curNode.x = x;
			curNode.y = y;
	//		print += "(" + to_string(x) +", " + to_string(y) + "), ";
			continue;
		}
		if(count == 0) {
			a = x;
			b = y;
		}
		count++;
//		checked.insert(curNode);	
		curNode.x = x;
		curNode.y = y;
	//	print += "(" + to_string(x) +", " + to_string(y) + "), ";	
	}	
	
	//a = checked.at(0).x;
	//b = checked.at(0).y;

	print += "Checked: ";
//	for(int i = 0; i < checked.size(); i++) {
		//print += "(" + to_string(checked.at(i).x) + ", " + to_string(checked.at(i).y) + "). ";
//	}
	printAtBottom("Search completed. x = " + to_string(a) + "...y = " + to_string(b)+". Count: " + 
		to_string(count) + "\n");
	printAtBottom(print);
	x = startX;
	y = startY;
}

void Ghost1::think() {
	if(GAME_WON != 0)
		return;
	if(THINKING)
	{
		writeError("ALREADY THINKING");
		usleep(sleepTime * 1000000 * 0.5);
		think();
		return;
	}

	THINKING = true;
	mtx.lock();
	

	stringstream msg;
	msg << sleepTime;
	writeError(msg.str());
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

	mtx.unlock();
	THINKING = false;
	usleep(sleepTime * 1000000);
	if(GAME_WON == 0)
		think();
}
/*
void Ghost1::think() { 
	if(THINKING) 
		usleep(sleepTime * 1000000 * 0.5);
	THINKING = true;
	int a = x, b = y;
//	backtrack(a, b);

	
//	moveTo(a, b, false);
	usleep(sleepTime * 1000000);
	if(GAME_WON == 0)
		think();
}

*/
void Ghost1::spawn() {
	letterUnder = charAt(x, y);
	if(!moveTo(x, y))
		return;
	
	sleep(1); // wait a second to create map, etc
	think();
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
	/*if(THINKING) {
		usleep(.05 * 1000000);
		onKeystroke(unit, key);
		return;
	}
	*/
	THINKING = true;
	mtx.lock();
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
	//else if(key == "n") {
		//gho.think();
	//}

	key = "";
	refresh();
	mtx.unlock();
	THINKING = false;
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

void ghostManager(Ghost1 &gho1, Ghost1 &gho2) {
	

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
//	player.moveRight();
	
	writeError("LOL");
	// Create ghost1
	Ghost1 ghost1(1, 1, .4, COLOR_BLUE);
	//thread ghostThread (&Ghost1::spawn, ghost1);

	// Ghost 2
	Ghost1 ghost2(8, 14, .3, COLOR_RED);

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
		printAtBottom(ss.str());
		move(player.getY(), player.getX());
		refresh();
	}	
	GAME_WON = -1;
	printf("AHHH");
	
	ghostThread1.join();
	ghostThread2.join();

	if(player.getPoints() >= TOTAL_POINTS) {
		winGame();
	}
	//endwin();
	clear();
	printf("GG"); refresh();
	sleep(.5);
	endwin();
	return 0;
}
