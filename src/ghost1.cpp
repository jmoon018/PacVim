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

void Ghost1::think() {
	while(GAME_WON == 0) {
		mtx.lock();
		std::stringstream msg;
		msg << sleepTime;
		//cout << "Thinking.." << endl;
		// evaluate the four potential paths and move accordingly
		double up = eval(x, y-1);
		double down = eval(x, y+1);
		double left = eval(x-1, y);
		double right = eval(x+1, y);

		if(up <= down && up <= left && up <= right)
			moveTo(x, y-1);
		else if(down <= left && down <= right && down <= up)
			moveTo(x, y+1);
		else if(left <= right && left <= up && left <= down)
			moveTo(x-1, y);
		else if(right <= up && right <= down && right <= left)
			moveTo(x+1, y);

		mtx.unlock();
		usleep(sleepTime * 1000000);
	}
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
void Ghost1::spawnGhost(bool spawned = false) { 
	mtx.lock();
	if(!moveTo(x, y) && !spawned) {
		mtx.unlock();
		return;
	}

	mtx.unlock();
	usleep(250000); // wait a quarter second
	writeError("TRYING TO SPAWN");
	if(!READY) {
		writeError("UNREADY!");
		spawnGhost(true);
		return;
	}
	writeError("SHOULD HAVE SPAWNED");
	think();
}

