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
