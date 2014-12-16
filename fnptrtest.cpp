#include <iostream>

using namespace std;

class human {
	private:
		int age;
		string name;
	public:
		human(int, string);
		int getAge();
		string getName();
};

human::human(int a, string n) {
	age = a;
	name = n;
}

int human::getAge() { return age; }
string human::getName() { return name; }

union myunion {
	int i;
	bool b;
	void *v;
};

template <typename T>
int print() {
	human jm = human(19, "Jamal");
	//int (*foo)();
	//foo = &human::getAge();
	typedef T (human::*METHOD) ();
	METHOD meth = &human::getAge;
	METHOD meth2 = &human::getName;
	//void (human::*t)() = &human::getAge();

	cout << "What is this human's name and age?" << endl;
	cout << (&jm->*meth2)() << ": " << (&jm->*meth)() << endl;

	return 0;
}


int main()
{
	print();
	return 0;
}
