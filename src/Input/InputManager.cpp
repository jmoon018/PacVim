#include "InputManager.h"
#include <iostream>
#include <limits>

std::string InputManager::mCurrentInput = "";

std::string InputManager::GetInput() {
	char input[256];
	std::cin.getline(input, 256);
	std::string inputStr(input);
	mCurrentInput += inputStr;
	return mCurrentInput;
}

void InputManager::ClearInputStream() {
	std::cin.ignore(std::numeric_limits<int>::max());
}

void InputManager::ClearCurrentInput() {
	mCurrentInput = "";
}

