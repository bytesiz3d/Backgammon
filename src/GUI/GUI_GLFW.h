#pragma once
#include "../Utility/Utility.h"
#include <GLFW/glfw3.h>

class GUI_GLFW
{
	// Interface Data Members:
	GLFWwindow *mWindow;

	int mScreenWidth, mScreenHeight,
		mCellWidth, mCellHeight,
		mTokenRadius, mBarWidth;

	Color mColors[STATUS_COUNT];

	// Mouse and flags:
	static int mX, mY;
	static bool mLeftClickf, mCancelf;
	static bool mSpacePressf;
	static bool mClosef;

	// Events:
	static void
	CursorPositionCallback(GLFWwindow *window, double xpos, double ypos);
	static void
	MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	static void
	KeyboardKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

	// Drawing Functions:
	void
	DrawBoard(PROGRAM_MODE CURRENT_MODE, Cell *board);
	void
	DrawCell(int i, const Cell &c);//i ==> cellNumber

	// Basic Shape/Utility Functions:
	void
	DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
	void
	DrawTriangle(Point p1, Point p2, Point p3);

	void
	DrawRectangle(int x1, int y1, int x2, int y2);
	void
	DrawRectangle(Point p1, Point p2);

	void
	DrawLine(int x1, int y1, int x2, int y2);
	void
	DrawLine(Point p1, Point p2);

	void
	DrawCircle(int x, int y, int radius, bool black);
	void
	DrawCircle(Point p, int radius, bool black);

	void
	SetColor(int r, int g, int b, bool background = false);
	void
	SetColor(Color c, bool background = false);

	float
	TriangleArea(int x1, int y1, int x2, int y2, int x3, int y3);
	float
	TriangleArea(Point p1, Point p2, Point p3);

public:
	GUI_GLFW();

	// Utility Function:
	int
	CellNumber();

	// Interface Functions:
	void
	PollEvents();
	void
	UpdateInteface(PROGRAM_MODE CURRENT_MODE, Cell *board, STATUS PLAYER);

	// Flags:
	bool
	Clicked();
	bool
	Cancelled();
	bool
	Rolled();
	bool
	Closed();

	inline void
	PickupToken()
	{}
	inline void
	DropToken()
	{}

	~GUI_GLFW();
};
