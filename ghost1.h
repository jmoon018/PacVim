#ifndef GHOST1_H
#define GHOST1_H

#include "avatar.h"
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
	//	void backtrack(int &a, int &b);
		void think();
};
#endif
