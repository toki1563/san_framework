#pragma once

class gameSystem
{
private:
	gameCamera* pGameCamera;

public:
	gameSystem();
	~gameSystem();
	void gameClear();
	void gameOver();
};