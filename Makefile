all: 
	g++ -w game.cpp -pthread -std=c++11 -lncurses globals.cpp helperFns.cpp avatar.cpp ghost1.cpp -o pacvim
