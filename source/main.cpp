#include "../GUI/GUI.h"
#include "../Game/Game.h"

int main(void)
{
	Game *pGame = new Game;
	
	pGame->Play();
	
	delete pGame;

	return 0;
}

