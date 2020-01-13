#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../Utility/Utility.h"

class GUI_SFML 
{    
    // Interface Data Members:
    sf::RenderWindow *mWindow;
    sf::Texture mTexture[2];
    
    int mScreenWidth, mScreenHeight,
	mCellWidth, mCellHeight,
	mTokenRadius, mBarWidth,
        mSpriteRadius;
    
    Color mColors[STATUS_COUNT];

    // Mouse and flags:
    static int mX, mY;
    static bool mLeftClickf, mCancelf;
    static bool mSpacePressf;
    static bool mClosef;
    static bool mPickupf;
    
    // Drawing Functions:
    void DrawBoard(PROGRAM_MODE CURRENT_MODE, Cell *board);
    void DrawCell(int i, const Cell& c); //i ==> cellNumber

    // Basic Shape/Utility Functions:
    float TriangleArea(int x1, int y1, int x2, int y2, int x3, int y3);
    float TriangleArea(Point p1, Point p2, Point p3);
    
public:
    
    GUI_SFML();
    
    // Utility Function:
    int CellNumber();

    // Interface Functions:
    void PollEvents();
    void UpdateInteface(PROGRAM_MODE CURRENT_MODE, Cell *board, STATUS PLAYER);

    // Flags:
    bool Clicked();
    bool Cancelled();
    bool Rolled();
    bool Closed();

    void PickupToken();
    void DropToken();

    ~GUI_SFML();
};

