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

#ifndef GHOST1_H
#define GHOST1_H

#include "avatar.h"
class Ghost1 : public avatar {
	private:
		double sleepTime;
		double eval();
		double eval(int a, int b);
	public:
		void lol();
		void spawnGhost(bool spawned);
		Ghost1(int a, int b, double c) : avatar(a, b) { sleepTime = c; }
		Ghost1(int a, int b) : avatar(a, b) { sleepTime = 0.5; }
		Ghost1() : avatar() { sleepTime = 0.5; }
		Ghost1(double time) : avatar() { sleepTime = time; }
		Ghost1(int a, int b, double c, int col) : avatar(a, b) { sleepTime = c; color = col; }
	//	void backtrack(int &a, int &b);
		void think();
};
#endif
