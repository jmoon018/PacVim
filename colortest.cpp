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
	attron(COLOR_PAIR(1) | A_UNDERLINE); //| A_UNDERLINE);
	move(5,5);
	addch('x');
	attroff(COLOR_PAIR(1) | A_UNDERLINE);

	chtype ch = mvinch(5,5);
	move(5,6);	
	if((ch) & (COLOR_PAIR(1)))
		addch(ch);

	stringstream ss;
	bitset<32> x (ch);
	ss << "\n" << x;
	printw ((ss.str()).c_str());	
	getch();
	endwin();
	return 0;
}
//  100000000101111000
