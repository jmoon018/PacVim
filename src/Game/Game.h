#ifndef GAME_H
#define GAME_H

#include <vector>
#include "../GameObject/GameObject.h"

class Game {
public:
	Game() : mIsGameOver(false) {}
	void StartGame();

private:
	void StartGameObjects();

	// Game loop
	void Update();
	void GetInput();
	void UpdateGameState();
	void Render();

	bool mIsGameOver; // if this is true, end the game loop
	std::vector<GameObjectPtr> mGameObjects;
};

#endif
