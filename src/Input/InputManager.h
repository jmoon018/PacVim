#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Input.h"
#include <string>

class InputManager {
public:
	static std::string GetInput();
	static void ClearCurrentInput();

private:
	static void ClearInputStream();

	static std::string mCurrentInput;
};

#endif //INPUTMANAGER_H
