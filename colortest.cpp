#include <cursesw.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <bitset>

using namespace std;

int main()
{
	initscr();
	start_color();
	clear();

	//init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(1, COLOR_GREEN, COLOR_BLACK );
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(6) | A_UNDERLINE); //| A_UNDERLINE);
	move(5,5);
	addch('x');
	attroff(COLOR_PAIR(6) | A_UNDERLINE);

	chtype ch = mvinch(5,5);
	move(5,6);	
	if((ch | COLOR_PAIR(5) == ch))
		addch(ch);

	stringstream ss;
	bitset<32> x (ch);
	bitset<32> y (COLOR_PAIR(2));
	bitset<32> a (COLOR_PAIR(3));
	bitset<32> z (COLOR_PAIR(6));
	bitset<32> b (ch | COLOR_PAIR(2));
	ss << "\n" << x << "\n" << y << "\n"<< y << "\n" << z;
	printw ((ss.str()).c_str());	
	cout << ((ch & COLOR_PAIR(6)) == COLOR_PAIR(2)) << endl;
	getch();
	endwin();
	return 0;
}
//  100000000101111000
