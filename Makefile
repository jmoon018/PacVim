all:
	$(CXX) src/game.cpp -pthread -std=c++11 -stdlib=libc++ -lncurses src/globals.cpp src/helperFns.cpp src/avatar.cpp src/ghost1.cpp -o pacvim
