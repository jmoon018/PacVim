#include "GameObject.h"

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
