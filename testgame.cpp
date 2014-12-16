#include <ncurses.h>
#include<unistd.h>
#include <iostream>
#include <time.h>
using namespace std;
int main()
{	char ch;

	initscr();			/* Start curses mode		*/

	while (ch != 'x')
	{
		printw("Type any characer to see it..\n");
		ch = getch();
		refresh();
	}
	sleep(5);
	endwin();
	return 0;
}
	
