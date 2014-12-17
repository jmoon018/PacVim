#include <thread>
#include <mutex>
#include <iostream>

using namespace std;

mutex mtx;

void fn1() {
	//mtx.lock();
	for(int i = 0; i < 1000; i++) {
		cout << "1" << flush;
	}
	//mtx.unlock();
}

void fn2() {
	//mtx.lock();
	for(int i = 0; i < 1000; i++) {
		cout << "2"<< flush;
	}
	//mtx.unlock();
}

int main() {	
	thread t1(fn2);
	thread t2(fn1);
	t1.join();
	t2.join();
	cout << "DONE" << endl;
	
	return 0;
}
