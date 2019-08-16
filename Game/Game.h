#pragma once

#if !defined(GUI)
	#if defined(_GUI_GLFW_)	
		#define GUI GUI_GLFW
	#elif defined(_GUI_SFML_)
		#define GUI GUI_SFML
	#endif
#endif

#if defined(_GUI_GLFW_)
	#include "../GUI/GUI_GLFW.h"
#elif defined(_GUI_SFML_)
	#include "../GUI/GUI_SFML.h"
#endif

//TODO: Implement Bearing Off

class Game
{
    GUI *pGUI;

    // Board
    Cell mCell[24];
        
    vector <int> mMoves;
        
    bool mBearingOff[2];

    PROGRAM_MODE mMode;

    void ResetAllCells(STATUS PLAYER);
    void ShowMoves(int click, STATUS PLAYER);

    bool Removable(STATUS PLAYER, int& largestMove, int &furthestCell);
    void ShowRemovableMoves(STATUS PLAYER, int largestMove, int furthestCell);
        
    void FillMoves();
    void RemoveChosenMove(int chosenCell, int previousCell, STATUS PLAYER);
    void DoMove(int previousCell, int chosenCell, STATUS PLAYER);

    void CheckBearingOff(STATUS PLAYER);

public:
    Game();

    void Play();
        
    ~Game();
};

