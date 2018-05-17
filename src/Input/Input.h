#ifndef INPUT_H
#define INPUT_H

#include "KeyBindings.h"
#include <vector>

class Input {
public:
	static KeyBinding GetKeyBindingFromKey(Key k);

	static std::vector<KeyBinding> GetKeyBindings();
private:

	// A list of all the keybindings in the game
	// Store it as a variable so we don't have to
	//    recalculate the list each time we want to use it
	static std::vector<KeyBinding> mKeyBindings;
};

#endif //INPUT_H
