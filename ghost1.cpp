#include "ghost1.h"

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
*/

void Ghost1::think() {
	if(GAME_WON != 0)
		return;
	if(THINKING)
	{
		usleep(sleepTime * 1000000 * 0.5);
		think();
		return;
	}

	THINKING = true;
	//mtx.lock();
	

	std::stringstream msg;
	msg << sleepTime;
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

	//mtx.unlock();
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
	if(!moveTo(x, y))
		return;

	sleep(2); // wait a second to create map, etc
	think();
}

