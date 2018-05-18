#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <string>

enum class Key {
	h,
	j,
	k,
	l,
	w,
	W,
	e,
	E,
	b,
	B,
	$,
	zero, // '0' character -- but can't use '0' alone in enum
	gg,
	G, // just 'G'
	NumG, // 1G, 24G, 2912G, etc
	Caret, // '^' key
	Ampersand, // '&'
	quit // ':q'
};

class KeyBinding {
public:
	KeyBinding(bool isRepeatable, bool hasNumberPrefix,
			   std::string commandString, Key key) :
		mCommandString(commandString),
		mIsRepeatable(isRepeatable),
		mHasNumberPrefix(hasNumberPrefix),
		mKey(key)
	{}

	bool IsRepeatable() { return mIsRepeatable; }
	bool HasNumberPrefix() { return mHasNumberPrefix; }
	std::string GetCommandString() { return mCommandString; }
	Key GetKey() { return mKey; }

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

	// Key enum
	Key mKey;
};

struct KeyBindingWrapper {
	KeyBindingWrapper(KeyBinding kb, unsigned reps) : mKeyBinding(kb), mRepetitions(reps) {}
	KeyBinding mKeyBinding;
	unsigned mRepetitions;
};


#endif //KEYBINDINGS_H
