#include <iostream>
#include <unistd.h>
#include <cursesw.h>
using namespace std;

int main() {
	initscr();
	start_color();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);

	attron(COLOR_PAIR(1));
	addch('~');
	attroff(COLOR_PAIR(1));

	chtype ch = mvinch(0,0);
	move(5,5);
	cout << ch << endl;
	getch();

	endwin();
	return 0;
}
