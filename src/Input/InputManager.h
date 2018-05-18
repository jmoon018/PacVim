#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Input.h"
#include <string>
#include <memory> // unique ptr

class InputManager {
public:
	static std::string GetInput();
	static KeyBindingWrapper* GetCurrentKeyBindingWrapper() { return mCurrentKeyBindingWrapper.get(); }
	static void ClearCurrentInput();

private:
	static void ClearCinBuffer();
	static void ResetCurrentKeyBindingWrapper();

	static void ProcessInput();
	static void SetKeyBindingWrapper(unsigned repetitions, KeyBinding keyBinding);

	static std::string mCurrentInput;
	static std::unique_ptr<KeyBindingWrapper> mCurrentKeyBindingWrapper;
};

// Helper functions
unsigned GetEndIndexOfPrecedingNumber(std::string command);
bool AreTwoCommandsTheSame(std::string inputStr, KeyBinding keyBinding);
unsigned GetNumberOfRepetitions(std::string inputStr, KeyBinding keyBinding);

#endif //INPUTMANAGER_H
