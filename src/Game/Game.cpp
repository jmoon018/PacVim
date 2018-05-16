#include "Game.h"
#include "../Input/InputManager.h"

using namespace std;

void Game::StartGame() {
	StartGameObjects();
	while (!mIsGameOver) {
		Update();
	}
}

void Game::StartGameObjects() {
	for (auto& gameObject: mGameObjects) {
		gameObject->Start();
	}
}


void Game::Update() {
	GetInput();
	UpdateGameState();
	Render();
}

void Game::GetInput() {
	InputManager::GetInput();
}

void Game::UpdateGameState() {
	for (auto& gameObject : mGameObjects) {
		gameObject->Update();
	}

	for (auto& gameObject : mGameObjects) {
		gameObject->LateUpdate();
	}
}

void Game::Render() { }