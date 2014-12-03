#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void f1(int x) {
	cout << "X" << flush;
	usleep(50 * 1000);
	if(x > 0)
		f1(x-1);
}

void f2(int x) {
	cout << "Y" << flush;
	usleep(50 * 1000);
	if(x > 0)
		f2(x-1);
}

int main() {
	thread (f1, 50).detach();
	thread (f2, 50).detach();

	cout << "Running concurrently..\n" << endl;

	//t1.join();
	//t2.join();

	int a = 5, b = 6, c = 7, d = 8;
	if (b <= c <= d <= a)
		cout << "B: " << b << endl;
	else if (a <= b <= c <= d)
		cout << "A: " << a << endl;
	sleep(5);
	return 0;
}
