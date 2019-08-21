#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../Utility/Utility.h"

class GUI_SFML 
{    
    sf::RenderWindow *mWindow;

    // Interface Data Members:
    int mScreenWidth, mScreenHeight,
	mCellWidth, mCellHeight,
	mTokenRadius, mBarWidth;

    Color mColors[STATUS_COUNT];

    // Mouse and flags:
    static int mX, mY;
    static bool mLeftClickf, mCancelf;
    static bool mSpacePressf;
    static bool mClosef;

    // Drawing Functions:
    void DrawBoard(PROGRAM_MODE CURRENT_MODE, Cell *board);
    void DrawCell(int i, const Cell& c); //i ==> cellNumber

    // Basic Shape/Utility Functions:
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
    void DrawTriangle(Point p1, Point p2, Point p3);

    void DrawRectangle(int x1, int y1, int x2, int y2);
    void DrawRectangle(Point p1, Point p2);

    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawLine(Point p1, Point p2);

    void DrawCircle(int x, int y, int radius, bool black);
    void DrawCircle(Point p, int radius, bool black);

    void SetColor(int r, int g, int b, bool background = false);
    void SetColor(Color c, bool background = false);

    float TriangleArea(int x1, int y1, int x2, int y2, int x3, int y3);
    float TriangleArea(Point p1, Point p2, Point p3);
    
public:
    
    GUI_SFML();
    
    // Utility Function:
    int CellNumber();

    // Interface Functions:
    void PollEvents();
    void UpdateInteface(PROGRAM_MODE CURRENT_MODE, Cell *board);

    // Flags:
    bool Clicked();
    bool Cancelled();
    bool Rolled();
    bool Closed();

    ~GUI_SFML();
};

