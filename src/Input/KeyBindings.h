#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <string>

class KeyBinding {
public:
	KeyBinding(bool isRepeatable, bool hasNumberPrefix, std::string commandString) :
		mIsRepeatable(isRepeatable),
		mHasNumberPrefix(hasNumberPrefix),
		mCommandString(commandString)
	{}

	bool IsRepeatable() { return mIsRepeatable; }
	bool HasNumberPrefix() { return mHasNumberPrefix; }
	std::string GetCommandString();

protected:
	// The string of characters that represent the command, WITHOUT preceding numbers
	std::string mCommandString;

	// if true, then the command can be repeated and prefixed by a number
	// eg: 5w, 6W, 2e, 124b, etc
	bool mIsRepeatable;

	// if true, then numbers can precede the command. Must be true for all repeatable
	// commands, but does not necessarily imply that the command is repeatable
	// EG: 5w, 6W, 2e, 124b, 24G, 2G, etc
	bool mHasNumberPrefix;
};


#endif //KEYBINDINGS_H
