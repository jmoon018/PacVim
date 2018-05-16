#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <string>

class InputManager {
public:
	static std::string GetInput();

private:
	static void ClearInputBuffer();

	static std::string mCurrentInput;
};

#endif //INPUTMANAGER_H
