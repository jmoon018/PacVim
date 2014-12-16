#include <iostream>

using namespace std;

// global cause it's super easy
const int ROW = 4;
const int COL = 6;
char board[ROW][COL] = {{"OOOOO"},
				   {"OO#OO"},
				   {"OO#OO"},
				   {"SO#OD"}};

void printBoard() {
	for(int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			cout << board[i][j];
		}
		cout << endl;
	}
}

bool isValid(int x, int y) {
	if(x < 0 || y < 0)
		return false; // out of bounds

	char found = board[x][y];
	if(found == '#')
		return false;
	
	return true;
}


void move(int x, int y) {
	board[x][y] = 'X';
}

void backtrack(int x, int y) {
	// try up
	if(isValid(x-1, y)) {

	}

}
int main() {
	
	printBoard();

	return 0;
}
