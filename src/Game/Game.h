#ifndef GAME_H
#define GAME_H

class Game {
public:
	Game() : mIsGameOver(false) {}
	void StartGame();

private:
	void Update();
	void GetInput();
	void UpdateGameState();
	void Render();

	bool mIsGameOver;
};

#endif
