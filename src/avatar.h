#ifndef AVATAR_H
#define AVATAR_H

#include <thread>
#include <math.h>
#include <sstream>
#include <unistd.h>
#include "helperFns.h"
// Avatar Class -- can be a ghost or player
class avatar {
	public:
		avatar();
		avatar(int, int);
		avatar(int, int, bool);
		avatar(int, int, bool, int);
	protected:
		chtype letterUnder;
		int x;
		int y;
		bool isPlayer;
		int points;
		char portrait;
		int lives;
		int color;
	public:	
		bool moveTo(int, int); 
		//bool moveTo(int, int, bool);
		bool moveRight();
		bool moveLeft();
		bool moveUp();
		bool moveDown();
		bool parseWordForward(bool);
		bool parseWordBackward(bool);
		bool parseWordEnd(bool);
		bool parseToBeginning();
		bool parseToEnd();

		int getPoints();
		bool getPlayer();
		int getX();
		int getY();
		bool setPos(int, int);
		char getPortrait();

		void setLetterUnder(char);
};
	
#endif
