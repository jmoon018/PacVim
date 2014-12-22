#include <iostream>
#include <vector>
#include <cursesw.h>
#include <fstream>

using namespace std;

void drawScreen() {
	ifstream in("map1.txt");
	vector<vector<chtype> > board;
	string str;
	vector<chtype> line;
	while(getline(in, str)) {	
		for(unsigned i = 0; i < str.length(); i++) {
			line.push_back(str[i]);
		}
		board.push_back(line);
		line.clear();	
	}
	for(unsigned i = 0; i < board.size(); i++) {
		for(unsigned j = 0; j < board.at(i).size(); j++) {
			addch(board.at(i).at(j));
		}
		addch('\n');
	}
}

int main() { 
	initscr();
	drawScreen();

	refresh();
	getch();
	endwin();
	return 0;
	/*
	initscr();

	addch(ACS_ULCORNER);

	int ch = mvinch(0,0);
	move(3, 3);
	if(ch == ACS_ULCORNER)
		addch(ch);
	else 
		addch('X');

	getch();
	endwin();
	*/

	return 0;
}
