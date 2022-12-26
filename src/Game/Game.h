#pragma once
#include "../GUI/GUI_SFML.h"

class Game
{
	GUI_SFML *pGUI;

	// Board
	Cell mCell[24];

	// Storage for moves
	std::vector<int> mMoves;

	// Bearing Off flags
	bool mBearingOff[2];

	// Current program state
	PROGRAM_MODE mMode;

	// Utility Functions:
	void
	ResetAllCells(STATUS PLAYER);
	void
	ShowMoves(int click, STATUS PLAYER);

	bool
	Removable(STATUS PLAYER, int &largestMove, int &furthestCell);
	void
	ShowRemovableMoves(STATUS PLAYER, int largestMove, int furthestCell);

	void
	FillMoves();
	void
	RemoveChosenMove(int chosenCell, int previousCell, STATUS PLAYER);
	void
	DoMove(int previousCell, int chosenCell, STATUS PLAYER);

	void
	CheckBearingOff(STATUS PLAYER);

public:
	Game();

	void
	Play();

	~Game();
};
