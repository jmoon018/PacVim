#include "InputManager.h"
#include <iostream>
#include <limits>
#include <regex>

std::string InputManager::mCurrentInput = "";
std::unique_ptr<KeyBindingWrapper> InputManager::mCurrentKeyBindingWrapper = nullptr;

std::string InputManager::GetInput() {
	char input[256];
	std::cin.getline(input, 256);
	std::string inputStr(input);
	mCurrentInput += inputStr;
	return mCurrentInput;
}

void InputManager::ClearCinBuffer() {
	std::cin.ignore(std::numeric_limits<int>::max());
}

void InputManager::ClearCurrentInput() {
	mCurrentInput = "";
}

void InputManager::ResetCurrentKeyBindingWrapper() {
	mCurrentKeyBindingWrapper.reset();
}


unsigned GetEndIndexOfPrecedingNumber(std::string command) {
	std::smatch regexMatch; // store result of match
	std::regex numsPrefixRegex("^[0-9]*"); // start of line, followed by any digits
	std::regex_search(command, regexMatch, numsPrefixRegex); // compare

	if (regexMatch.empty()) {
		return 0;
	} else {
		return regexMatch.length(0);
	}
}

bool AreTwoCommandsTheSame(std::string inputStr, KeyBinding keyBinding) {
	if (keyBinding.HasNumberPrefix()) {
		unsigned index = GetEndIndexOfPrecedingNumber(inputStr);
		std::string inputWithoutPrefixedNums = inputStr.substr(index, std::string::npos);
		return inputWithoutPrefixedNums == keyBinding.GetCommandString();
	} else {
		return inputStr == keyBinding.GetCommandString();
	}
}

unsigned GetNumberOfRepetitions(std::string inputStr, KeyBinding keyBinding) {
	if (keyBinding.IsRepeatable()) {
		unsigned endOfNumberIndex = GetEndIndexOfPrecedingNumber(inputStr);
		std::string digitsStr = inputStr.substr(0, endOfNumberIndex-1);
		unsigned repetitions = static_cast<unsigned>(std::stoi(digitsStr));
		return repetitions;
	} else {
		return 0;
	}
}

void InputManager::ProcessInput() {
	// iterate through each character in the buffer
	std::string curInputStr;
	for (unsigned i = 0; i < GetInput().size(); ++i) {
		char inputChar = GetInput().at(i);
		curInputStr += inputChar;

		// iterate through KeyBindings; maintain list of KeyBindings that could match the input
		std::vector<KeyBinding> matchingKeyBindings;
		for (auto& keybinding : Input::GetKeyBindings()) {
			std::string keybindingStr = keybinding.GetCommandString();

			// if the command can be preceded by numbers, determine where the command actually begins
			unsigned numsPrefixLength = 0;
			if (keybinding.HasNumberPrefix()) {
				numsPrefixLength = GetEndIndexOfPrecedingNumber(curInputStr);

				// This is true when the input is just numbers, with no character (eg: "52")
				// When this occurs, and the KeyBinding has preceding numbers, this counts
				//    as a potential match, so we want to add it to the list
				if (numsPrefixLength == curInputStr.size()) {
					matchingKeyBindings.push_back(keybinding);
					continue;
				}
			}

			// compare input and keybinding string; check if input string is contained
			//    within the keybinding string (with preceding numbers ignored from curInputStr)
			size_t matchPos = keybindingStr.find(
				curInputStr.substr(numsPrefixLength, std::string::npos)
			);

			if (matchPos != std::string::npos) {
				// found a match; add it to the list
				matchingKeyBindings.push_back(keybinding);
			}
		}

		// We have no matching keybind, so delete this part of the buffer
		if (matchingKeyBindings.size() == 0) {
			mCurrentInput.erase(0, curInputStr.size());
			i-= curInputStr.size();
			curInputStr = "";
			ResetCurrentKeyBindingWrapper();
		} else if (matchingKeyBindings.size() > 1) {
			// could have more than one potential match
			ResetCurrentKeyBindingWrapper();
		} else if (matchingKeyBindings.size() == 1) {
			// make a final check that the two match
			KeyBinding matchingKeyBinding = matchingKeyBindings.at(0);
			if (AreTwoCommandsTheSame(curInputStr, matchingKeyBinding)) {
				unsigned repetitionCnt = GetNumberOfRepetitions(curInputStr, matchingKeyBinding);
				SetKeyBindingWrapper(repetitionCnt, matchingKeyBinding);
			} else {
				ResetCurrentKeyBindingWrapper();
			}
		}
	}
}

void InputManager::SetKeyBindingWrapper(unsigned repetitions, KeyBinding keyBinding) {
	ResetCurrentKeyBindingWrapper();
	mCurrentKeyBindingWrapper = std::unique_ptr<KeyBindingWrapper>(
		new KeyBindingWrapper(keyBinding, repetitions)
	);
}