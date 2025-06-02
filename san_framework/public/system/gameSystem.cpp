#include "../../framework.h"
#include "../../framework/san_environment.h"

gameSystem::gameSystem()
{
	pGameCamera = new gameCamera();
}

gameSystem::~gameSystem()
{
	delete pGameCamera;
}

void gameSystem::gameClear()
{

}

void gameSystem::gameOver()
{

}