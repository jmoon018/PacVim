#include "Input.h"
#include <stdexcept>

// Initialize static var
std::vector<KeyBinding> Input::mKeyBindings = std::vector<KeyBinding>();

KeyBinding Input::GetKeyBindingFromKey(Key k) {
	for (auto& keyBinding : GetKeyBindings()) {
		if (keyBinding.GetKey() == k) {
			return keyBinding;
		}
	}

	throw std::runtime_error("Could not find KeyBinding for key");

	// return a dummy keybinding; the program should crash before then, though
	return KeyBinding(false, false, "ERROR", Key::quit);
}

std::vector<KeyBinding> Input::GetKeyBindings() {
	if (mKeyBindings.size() == 0) {
		std::vector<KeyBinding> keybindings {
			KeyBinding(true, true, "h", Key::h),
			KeyBinding(true, true, "j", Key::j),
			KeyBinding(true, true, "k", Key::k),
			KeyBinding(true, true, "l", Key::l),
			KeyBinding(true, true, "w", Key::w),
			KeyBinding(true, true, "W", Key::W),
			KeyBinding(true, true, "b", Key::b),
			KeyBinding(true, true, "B", Key::B),
			KeyBinding(true, true, "e", Key::e),
			KeyBinding(true, true, "E", Key::E),
			KeyBinding(false, false, "$", Key::$),
			KeyBinding(false, false, "0", Key::zero),
			KeyBinding(false, false, "gg", Key::gg),
			KeyBinding(false, false, "G", Key::G),
			KeyBinding(false, true, "G", Key::NumG),
			KeyBinding(false, false, "^", Key::Caret),
			KeyBinding(false, false, "&", Key::Ampersand), // for cheatz
			KeyBinding(false, false, ":q", Key::quit)
		};
	}
	return mKeyBindings;
}
