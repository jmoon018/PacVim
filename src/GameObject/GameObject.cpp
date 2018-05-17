#include "GameObject.h"
//#include "memory"


// Component-related functions
// ===========================================================

void GameObject::AddComponent(ComponentPtr component) {
	mComponents.push_back(component);
}

template<class T>
T* GameObject::GetComponentOfType() {
	// This is probably inefficient and bad, but whatever
	for (auto& component : mComponents) {
		T* desiredTypeComponent = dynamic_cast<T>(component);
		if (desiredTypeComponent != nullptr) {
			return desiredTypeComponent;
		}
	}
	return nullptr;
}



// Methods called for each component on the first or on each frame
// ==============================================================
void GameObject::Start() {
	for (auto& component : mComponents) {
		component->Start();
	}
}

void GameObject::Update() {
	for (auto& component : mComponents) {
		component->Update();
	}
}

void GameObject::LateUpdate() {
	for (auto& component : mComponents) {
		component->LateUpdate();
	}
}
