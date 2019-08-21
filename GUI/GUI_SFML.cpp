#include "GUI_SFML.h"

int GUI_SFML::mX = 0;
int GUI_SFML::mY = 0;

bool GUI_SFML::mClosef = false;

bool GUI_SFML::mLeftClickf = false;
bool GUI_SFML::mCancelf = false;

bool GUI_SFML::mSpacePressf = false;


GUI_SFML::GUI_SFML() :
    mScreenWidth(1280), mScreenHeight(900), mCellWidth(100), mCellHeight(360),
    mTokenRadius(40), mBarWidth(80)
{
    srand(time(0));

    mColors[HIGHLIGHTED] = "#B4D5FF";
    mColors[IDLE] = "#381704";
    mColors[SELECTED] = "#00FF00";
    mColors[BLACK] = 0x000000;
    mColors[WHITE] = 0xFFFFFF;

    mWindow = new sf::RenderWindow(sf::VideoMode(mScreenWidth, mScreenHeight), "Backgammon!", sf::Style::Titlebar | sf::Style::Close);

}

int GUI_SFML::CellNumber()
{
    // P (mX, mY)
    Point P = { mX, mY };

    float At, A1, A2, A3;

    for (int i = 0; i < 6; i++)
    {
        //Top Left:
        Point A = { mCellWidth * i, 0 };
        Point B = { mCellWidth * (1 + i), 0 };
        Point C = { mCellWidth * (0.5 + i), mCellHeight };

        At = TriangleArea(A, B, C);
        A1 = TriangleArea(P, B, C);
        A2 = TriangleArea(P, A, C);
        A3 = TriangleArea(P, A, B);

        if (At == A1 + A2 + A3)
            return i;

        //Top Right:
        A.x += 6 * mCellWidth + mBarWidth;
        B.x += 6 * mCellWidth + mBarWidth;
        C.x += 6 * mCellWidth + mBarWidth;

        At = TriangleArea(A, B, C);
        A1 = TriangleArea(P, B, C);
        A2 = TriangleArea(P, A, C);
        A3 = TriangleArea(P, A, B);

        if (At == A1 + A2 + A3)
            return i + 6;

        //Bottom Right:
        A.y = B.y = mScreenHeight;
        C.y = mScreenHeight - mCellHeight;

        At = TriangleArea(A, B, C);
        A1 = TriangleArea(P, B, C);
        A2 = TriangleArea(P, A, C);
        A3 = TriangleArea(P, A, B);

        if (At == A1 + A2 + A3)
            return 17 - i;

        //Bottom Left
        A.x -= 6 * mCellWidth + mBarWidth;
        B.x -= 6 * mCellWidth + mBarWidth;
        C.x -= 6 * mCellWidth + mBarWidth;

        At = TriangleArea(A, B, C);
        A1 = TriangleArea(P, B, C);
        A2 = TriangleArea(P, A, C);
        A3 = TriangleArea(P, A, B);

        if (At == A1 + A2 + A3)
            return 23 - i;
    }

    return -1;
}

void GUI_SFML::PollEvents()
{
    // Updating the mouse position:
    mX = sf::Mouse::getPosition(mWindow).x;
    mY = sf::Mouse::getPosition(mWindow).y;

    sf::Event ev;
    
    while (mWindow.pollEvent(ev))
	switch (ev.type)
	{
	case sf::Event::Closed:
	    mClosef = true;
	    break;

	case sf::Event::MouseButtonPressed:
	    switch (ev.mouseButton.button)
	    {
	    case sf::Mouse::Left:
		mLeftClickf = true;
		break;

	    case sf::Mouse::Right:
		mCancelf = true;
		break;
	    }
	    break;

	case sf::Event:KeyPressed:
	    switch (ev.key.code)
	    {
	    case sf::Keyboard::Space:
		mSpacePressf = true;
		break;

	    case sf::Keyboard::Escape:
		mCancelf = true;
		break;
	    }
	    break;
	}
    
}

void GUI_SFML::UpdateInteface(PROGRAM_MODE CURRENT_MODE, Cell * board)
{
    mLeftClickf = false;
    mCancelf = false;
    mSpacePressf = false;

    DrawBoard(CURRENT_MODE, board);
    // "Show"
}

bool GUI_SFML::Clicked()
{
    return mLeftClickf;
}

bool GUI_SFML::Cancelled()
{
    return mCancelf;
}

bool GUI_SFML::Rolled()
{
    return mSpacePressf;
}

bool GUI_SFML::Closed()
{
    return mCancelf;
}

GUI_SFML::~GUI_SFML()
{
    delete mWindow;
}

void GUI_SFML::DrawBoard(PROGRAM_MODE CURRENT_MODE, Cell * board)
{
}

void GUI_SFML::DrawCell(int i, const Cell & c)
{
}

void GUI_SFML::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
}

void GUI_SFML::DrawTriangle(Point p1, Point p2, Point p3)
{
}

void GUI_SFML::DrawRectangle(int x1, int y1, int x2, int y2)
{
}

void GUI_SFML::DrawRectangle(Point p1, Point p2)
{
}

void GUI_SFML::DrawLine(int x1, int y1, int x2, int y2)
{
}

void GUI_SFML::DrawLine(Point p1, Point p2)
{
}

void GUI_SFML::DrawCircle(int x, int y, int radius, bool black)
{
}

void GUI_SFML::DrawCircle(Point p, int radius, bool black)
{
}

void GUI_SFML::SetColor(int r, int g, int b, bool background)
{
}

void GUI_SFML::SetColor(Color c, bool background)
{
}

float GUI_SFML::TriangleArea(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return abs((x1*(y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

float GUI_SFML::TriangleArea(Point p1, Point p2, Point p3)
{
    return TriangleArea(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}
