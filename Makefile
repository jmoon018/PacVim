all:
	$(CXX) src/*.cpp -pthread -std=c++11 -stdlib=libc++ -lncurses -o pacvim
