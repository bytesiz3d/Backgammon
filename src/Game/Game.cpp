#include "Game.h"

// Removes highlighting from all cells
void
Game::ResetAllCells(STATUS PLAYER)
{
	for (auto &c : mCell)
		if (c.status == HIGHLIGHTED || c.status == SELECTED)
			if (c.tokenCount == 0)
				c.status = IDLE, c.player = IDLE;
			else
				c.status = PLAYER, c.player = PLAYER;
}

// Highlights all available moves
void
Game::ShowMoves(int click, STATUS PLAYER)
{
	switch (PLAYER)
	{
	case WHITE:
		for (int m = 0; m < mMoves.size(); m++)
		{
			if (click + mMoves[m] < 24)
				if (mCell[click + mMoves[m]].status == IDLE || mCell[click + mMoves[m]].status == PLAYER)
					mCell[click + mMoves[m]].status = HIGHLIGHTED;
		}
		break;

	case BLACK:
		for (int m = 0; m < mMoves.size(); m++)
		{
			if (click - mMoves[m] >= 0)
				if (mCell[click - mMoves[m]].status == IDLE || mCell[click - mMoves[m]].status == PLAYER)
					mCell[click - mMoves[m]].status = HIGHLIGHTED;
		}
		break;
	}
}

// Returns true if a piece can be taken off the board
bool
Game::Removable(STATUS PLAYER, int &largestMove, int &furthestCell)
{
	furthestCell = 999;
	largestMove = -1;

	// Find the largest move
	for (auto move : mMoves)
		largestMove = (move > largestMove) ? move : largestMove;

	// Find the furthest cell from the edge
	// If a piece matches a move, return true
	switch (PLAYER)
	{
	case WHITE:

		for (int c = 18; c < 24; c++)
			if (mCell[c].status == WHITE)
			{
				furthestCell = 24 - c;
				break;
			}

		for (auto move : mMoves)
		{
			if (mCell[24 - move].status == WHITE)
				return true;
		}

		break;

	case BLACK:

		for (int c = 5; c > -1; c--)
			if (mCell[c].status == BLACK)
			{
				furthestCell = c - (-1);
				break;
			}

		for (auto move : mMoves)
		{
			if (mCell[-1 + move].status == BLACK)
				return true;
		}

		break;
	}
	// If no piece matches a move, return true if the largest move can
	// remove the furthest piece
	return largestMove > furthestCell;
}

// Shows all pieces that can be taken off the board
void
Game::ShowRemovableMoves(STATUS PLAYER, int largestMove, int furthestCell)
{
	// If the largest move can remove the furthest cell, highlight everything
	if (largestMove > furthestCell)
	{
		switch (PLAYER)
		{
		case WHITE:

			for (int i = 24 - furthestCell; i < 24; i++)
				if (mCell[i].status == WHITE)
					mCell[i].status = HIGHLIGHTED;

			break;

		case BLACK:

			for (int i = -1 + furthestCell; i > -1; i--)
				if (mCell[i].status == BLACK)
					mCell[i].status = HIGHLIGHTED;

			break;
		}
	}

	// And if not, highlight available moves
	else
	{
		switch (PLAYER)
		{
		case WHITE:

			for (auto move : mMoves)
			{
				if (mCell[24 - move].status == WHITE)
					mCell[24 - move].status = HIGHLIGHTED;
			}

			break;

		case BLACK:

			for (auto move : mMoves)
			{
				if (mCell[-1 + move].status == BLACK)
					mCell[-1 + move].status = HIGHLIGHTED;
			}

			break;
		}
	}
}

// Fills the Moves vector
void
Game::FillMoves()
{
	std::cout << "\n Rolling!\n";

	// "Roll" twice
	mMoves.push_back(rand() % 6 + 1);
	mMoves.push_back(rand() % 6 + 1);
	std::cout << "\n " << mMoves[0] << " " << mMoves[1];

	// Add two additional moves if a pair is rolled
	if (mMoves[0] == mMoves[1])
	{
		mMoves.push_back(mMoves[0]);
		mMoves.push_back(mMoves[0]);
		std::cout << " " << mMoves[0] << " " << mMoves[0];
	}
	std::cout << " \n";
}

// Removes the chosen move from the Moves vector
void
Game::RemoveChosenMove(int previousCell, int chosenCell, STATUS PLAYER)
{
	int validMove = abs(chosenCell - previousCell);
	std::vector<int>::iterator moveItr = mMoves.begin();

	// If the move is found, remove it
	for (; moveItr < mMoves.end(); moveItr++)
		if (*moveItr == validMove)
			break;

	// If not, find the smallest possible move that is larger than the chosen one,
	// and remove it
	if (moveItr == mMoves.end())
	{
		moveItr = mMoves.begin();
		int smallestValidMove = 999;

		for (std::vector<int>::iterator it = mMoves.begin(); it < mMoves.end(); it++)
		{
			if (*it > validMove && *it < smallestValidMove)
			{
				smallestValidMove = *it;
				moveItr = it;
			}
		}
	}

	mMoves.erase(moveItr);
}

// Moves the piece between two cells (or off the board)
void
Game::DoMove(int previousCell, int chosenCell, STATUS PLAYER)
{
	if (--mCell[previousCell].tokenCount == 0)
		mCell[previousCell].status = IDLE, mCell[previousCell].player = IDLE;
	else
		mCell[previousCell].status = PLAYER, mCell[previousCell].player = PLAYER;

	// If the selected cell isn't an edge, place the piece
	if (chosenCell > -1 && chosenCell < 24)
	{
		mCell[chosenCell].tokenCount++;
		mCell[chosenCell].status = PLAYER;
		mCell[chosenCell].player = PLAYER;
	}
}

// Sets the Bearing Off flags for both players
void
Game::CheckBearingOff(STATUS PLAYER)
{
	switch (PLAYER)
	{
	case WHITE:

		for (int i = 0; i <= 18; i++)
		{
			if (i == 18)
			{
				mBearingOff[WHITE] = true;
				break;
			}

			if (mCell[i].status == WHITE)
			{
				mBearingOff[WHITE] = false;
				break;
			}
		}

		break;

	case BLACK:

		for (int i = 23; i >= 5; i--)
		{
			if (i == 5)
			{
				mBearingOff[BLACK] = true;
				break;
			}

			if (mCell[i].status == BLACK)
			{
				mBearingOff[BLACK] = false;
				break;
			}
		}

		break;
	}
}

Game::Game()
	: mMode(STANDBY)
{
	pGUI = new GUI;
	mBearingOff[0] = mBearingOff[1] = false;

	mCell[0] = Cell(WHITE, WHITE, 15);
	mCell[23] = Cell(BLACK, BLACK, 15);
}

// Main Game Loop
void
Game::Play()
{
	mMoves.resize(0);

	int prevCell = -1;
	int click = -1;

	int largestMove = -1;
	int furthestCell = 999;

	STATUS PLAYER = IDLE;

	// Roll to decide the starting player.
	do
	{
		mMoves.clear();
		FillMoves();
		if (mMoves[0] > mMoves[1])
			PLAYER = WHITE;
		else if (mMoves[1] > mMoves[0])
			PLAYER = BLACK;
	} while (mMoves[0] == mMoves[1]);
	mMoves.clear();

	std::cout << "\n--------------------------------------------\n\n>> "
			  << ((PLAYER == WHITE) ? "White's turn\n" : "Black's turn\n");

	/*
	  Poll Events
	  Handle Events
	  Update Data
	  Render Data
	  ---> in less than 16 ms
	*/

	while (!pGUI->Closed())
	{

		// Poll for and process events
		pGUI->PollEvents();

		click = pGUI->CellNumber();

		switch (mMode)
		{
		case STANDBY:

			// If I didn't roll
			if (!pGUI->Rolled())
				break;

			// Fill moves
			FillMoves();

			// Start Bearing Off
			if (mBearingOff[PLAYER])
				mMode = BEARING_OFF;

			// Start Piece Selection
			else
				mMode = PIECE_SELECTION;

			break;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		case PIECE_SELECTION:

			// Manual Break
			if (pGUI->Cancelled())
			{
				// Reset all cells
				ResetAllCells(PLAYER);

				// Reset all moves
				mMoves.clear();

				// Change turn
				std::cout << "\n--------------------------------------------\n";
				PLAYER = (STATUS)!PLAYER;
				std::cout << "\n>> " << ((PLAYER == WHITE) ? "White's turn\n" : "Black's turn\n");

				// Standby
				mMode = STANDBY;
				break;
			}

			// Ran out of moves
			if (mMoves.empty())
			{
				// Reset all cells
				ResetAllCells(PLAYER);

				// Change turn
				std::cout << "\n>> End of turn\n\n--------------------------------------------\n";
				PLAYER = (STATUS)!PLAYER;
				std::cout << "\n>> " << ((PLAYER == WHITE) ? "White's turn\n" : "Black's turn\n");

				// Standby
				mMode = STANDBY;
				break;
			}

			// Didn't click or clicked wrong
			if (!pGUI->Clicked() || click < 0)
				break;

			// Clicked an invalid cell
			if (mCell[click].status == IDLE)
				break;

			if (mCell[click].status == (STATUS)!PLAYER)
			{
				std::cout << "\n Cell " << click + 1 << " has " << mCell[click].tokenCount << " piece";
				if (mCell[click].tokenCount > 1)
					std::cout << "s";
				std::cout << "\n";
				break;
			}

			// Show moves
			ShowMoves(click, PLAYER);

			std::cout << "\n Cell " << click + 1 << " has " << mCell[click].tokenCount << " piece";
			if (mCell[click].tokenCount > 1)
				std::cout << "s";
			std::cout << "\n";

			// Record current cell
			prevCell = click;
			mCell[click].status = SELECTED;

			// Start Move Selection
			mCell[prevCell].tokenCount--;
			mMode = MOVE_SELECTION;

			break;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		case MOVE_SELECTION:

			// Cancelled
			if (pGUI->Cancelled())
			{
				// Reset all cells
				mCell[prevCell].tokenCount++;
				pGUI->DropToken();
				ResetAllCells(PLAYER);

				// Return to Piece Selection
				mMode = PIECE_SELECTION;
				break;
			}

			// Pick up the last token
			pGUI->PickupToken();

			// Didn't click or clicked wrong
			if (!pGUI->Clicked() || click < 0)
				break;

			// Clicked an invalid cell
			if (mCell[click].status != HIGHLIGHTED)
				break;

			// Remove the chosen move
			RemoveChosenMove(prevCell, click, PLAYER);

			// Move the current cell
			mCell[prevCell].tokenCount++;
			DoMove(prevCell, click, PLAYER);

			// Reset all cells
			ResetAllCells(PLAYER);

			// Drop the token
			pGUI->DropToken();

			// Check Bearing Off
			if (!mBearingOff[PLAYER])
				CheckBearingOff(PLAYER);

			// Start Bearing Off
			if (mBearingOff[PLAYER])
				mMode = BEARING_OFF;

			// Start Piece Selection
			else
				mMode = PIECE_SELECTION;

			break;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		case BEARING_OFF:

			// Manual Break
			if (pGUI->Cancelled())
			{
				// Reset all cells
				ResetAllCells(PLAYER);

				// Reset all moves
				mMoves.clear();

				// Change turn
				std::cout << "\n--------------------------------------------\n";
				PLAYER = (STATUS)!PLAYER;
				std::cout << "\n>> " << ((PLAYER == WHITE) ? "White's turn\n" : "Black's turn\n");

				// Standby
				mMode = STANDBY;
				break;
			}

			// Ran out of moves
			if (mMoves.empty())
			{
				// Reset all cells
				ResetAllCells(PLAYER);

				// Change turn
				std::cout << "\n>> End of turn\n\n--------------------------------------------\n";
				PLAYER = (STATUS)!PLAYER;
				std::cout << "\n>> " << ((PLAYER == WHITE) ? "White's turn\n" : "Black's turn\n");

				// Standby
				mMode = STANDBY;
				break;
			}

			if (Removable(PLAYER, largestMove, furthestCell))
			{
				ShowRemovableMoves(PLAYER, largestMove, furthestCell);
				mMode = BO_MOVE_SELECTION;
				break;
			}
			else
			{
				mMode = PIECE_SELECTION;
				break;
			}

			break;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		case BO_MOVE_SELECTION:

			// Cancelled
			if (pGUI->Cancelled())
			{
				// Reset all cells
				ResetAllCells(PLAYER);

				// Reset all moves
				mMoves.clear();

				// Change turn
				std::cout << "\n--------------------------------------------\n";
				PLAYER = (STATUS)!PLAYER;
				std::cout << "\n>> " << ((PLAYER == WHITE) ? "White's turn\n" : "Black's turn\n");

				// Standby
				mMode = STANDBY;
				break;
			}

			// Didn't click or clicked wrong
			if (!pGUI->Clicked() || click < 0)
				break;

			// Clicked an invalid cell
			if (mCell[click].status != HIGHLIGHTED)
				break;

			// Remove the chosen move
			int edge = (PLAYER == WHITE) ? 24 : -1;
			RemoveChosenMove(click, edge, PLAYER);

			// Move the current cell
			DoMove(click, edge, PLAYER);

			// Reset all cells
			ResetAllCells(PLAYER);

			// Return to Bearing Off
			mMode = BEARING_OFF;
			break;
		}

		pGUI->UpdateInteface(mMode, mCell, PLAYER);
	}
}

Game::~Game()
{
	mMoves.clear();
	delete pGUI;
}
