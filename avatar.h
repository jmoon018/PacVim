#ifndef AVATAR_H
#define AVATAR_H

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
		bool parseWordForward(bool);
		bool parseWordBackward(bool);
		bool parseWordEnd(bool);

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
	letterUnder = charAt(a, b);
	moveTo(a, b, false);
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
	letterUnder = charAt(a, b);
	moveTo(a, b);
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
	writeAt(x, y, letterUnder, COLOR_GREEN);
	x = a;
	y = b;

	chtype curChar = charAt(a, b); 
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
	
	if(curChar != ' ' && !(curChar & COLOR_PAIR(2)))
		points++;
	
	if(points >= TOTAL_POINTS) {
		winGame();
	}
	//printAtBottom("moving player");
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

	chtype curChar = charAt(a, b);
	if(del)
	{
		writeAt(a, b, portrait, color);
		writeAt(x, y, letterUnder, COLOR_BLUE);
		move(35, 0);
		addch(curChar);
		move(b,a);
		if(curChar != ' ' && !(curChar & COLOR_PAIR(2)))
			points++;
		if(points >= TOTAL_POINTS) {
			winGame();	
		}
	}
	else {
		writeAt(a, b, portrait, color);
		writeAt(x, y, letterUnder, COLOR_WHITE);
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

bool avatar::parseWordEnd(bool isWord) {
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
				return false;
			if(nextChar != ' ')
				breakOnSpace = true;
			nextChar = charAt(x+1, y);
		}
	}
	return true;
}
				
bool avatar::parseWordBackward(bool isWord) {
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
				return false;
			if(nextChar != ' ')
				breakOnSpace = true;
			nextChar = charAt(x-1, y); 
		}
	}
	return true;
}

bool avatar::parseWordForward(bool isWord) {
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
				return false;
			curChar = charAt(x,y);
		}
	}
	return true;
}

#endif
