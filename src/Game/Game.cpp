#include "Game.h"
#include "../Input/InputManager.h"

void Game::StartGame() {
	while (!mIsGameOver) {
		Update();
	}
}

void Game::Update() {
	GetInput();
	UpdateGameState();
	Render();
}

void Game::GetInput() { }

void Game::UpdateGameState() { }

void Game::Render() { }