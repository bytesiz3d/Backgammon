#include "GUI_SFML.h"

int GUI_SFML::mX = 0;
int GUI_SFML::mY = 0;

bool GUI_SFML::mClosef = false;

bool GUI_SFML::mLeftClickf = false;
bool GUI_SFML::mCancelf = false;

bool GUI_SFML::mSpacePressf = false;
bool GUI_SFML::mPickupf = false;

GUI_SFML::GUI_SFML() :
    mScreenWidth(1280), mScreenHeight(900), mCellWidth(100), mCellHeight(360),
    mTokenRadius(45), mBarWidth(80),
    mSpriteRadius(45)
{
    srand(time(0));

    // Setting the color environment:
    mColors[HIGHLIGHTED] = "#B4D5FF";
    mColors[IDLE] = "#381704";
    mColors[SELECTED] = "#00FF00";
    mColors[BLACK] = mColors[IDLE];
    mColors[WHITE] = mColors[IDLE];

    // Initializing the window:
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    mWindow = new sf::RenderWindow(sf::VideoMode(mScreenWidth, mScreenHeight), "Backgammon!", sf::Style::Titlebar | sf::Style::Close, settings);

    // Loading the textures:
    if (!mTexture[WHITE].loadFromFile("../assets/white-90.png") ||
	!mTexture[BLACK].loadFromFile("../assets/black-90.png"))
    {
	std::cerr << "Failed to load textures\n";
	mClosef = true;
    }
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
    mX = sf::Mouse::getPosition(*mWindow).x;
    mY = sf::Mouse::getPosition(*mWindow).y;

    sf::Event ev;
    
    while (mWindow->pollEvent(ev))
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

	case sf::Event::KeyPressed:
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

void GUI_SFML::UpdateInteface(PROGRAM_MODE CURRENT_MODE, Cell * board, STATUS PLAYER)
{
    mLeftClickf = false;
    mCancelf = false;
    mSpacePressf = false;

    // Draw:
    mWindow->clear();
    DrawBoard(CURRENT_MODE, board);

    // Handling the token:
    if (mPickupf)
    {
	sf::Sprite token;
	token.setTexture(mTexture[PLAYER]);

        token.scale(sf::Vector2f((float)mTokenRadius / mSpriteRadius,
                                 (float)mTokenRadius / mSpriteRadius));

        token.setOrigin(sf::Vector2f((float)mSpriteRadius, (float)mSpriteRadius));

        token.setPosition(sf::Vector2f(sf::Mouse::getPosition(*mWindow).x,
                                       sf::Mouse::getPosition(*mWindow).y));

	mWindow->draw(token);
    }
    
    // Swap Buffers:
    mWindow->display();
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
    return mClosef;
}

GUI_SFML::~GUI_SFML()
{
    delete mWindow;
}

void GUI_SFML::DrawBoard(PROGRAM_MODE CURRENT_MODE, Cell * board)
{
    sf::Color currentColor;
    switch (CURRENT_MODE)
    {
    case STANDBY:
	currentColor = sf::Color(0xF1, 0xC3, 0x8E, 0xFF);
	break;

    case PIECE_SELECTION:
        currentColor = sf::Color(0xD2, 0x6A, 0xD9, 0xFF);
        break;

    case MOVE_SELECTION:
        currentColor = sf::Color(0x0B, 0xA4, 0x94, 0xFF);
        break;
        
    case BO_MOVE_SELECTION:
        currentColor = sf::Color(0xFF, 0x00, 0x00, 0xFF);
        break;
    }

    // Draw the background
    mWindow->clear(currentColor);
    
    // Draw the board
    for (int i = 0; i < 24; i++)
        DrawCell(i, board[i]);

    // Draw the middle separator
    currentColor = sf::Color(mColors[IDLE].r, mColors[IDLE].g, mColors[IDLE].b, 0xFF);

    sf::RectangleShape middleBar(sf::Vector2f(mBarWidth - 16, mScreenHeight));
    middleBar.setPosition((mScreenWidth - (mBarWidth - 16)) / 2, 0);
    middleBar.setFillColor(currentColor);

    middleBar.setOutlineThickness(4);
    middleBar.setOutlineColor(sf::Color::Black);

    mWindow->draw(middleBar);

    sf::RectangleShape middleLine(sf::Vector2f(4, mScreenHeight));
    middleLine.setPosition(mScreenWidth / 2 - 2, 0);
    middleLine.setFillColor(sf::Color::Black);

    mWindow->draw(middleLine);
}

void GUI_SFML::DrawCell(int i, const Cell & c)
{
    int quad = i / 6;
    Color cellColor = mColors[c.status];

    i %= 6;

    // Locating the three points
    Point p1 = { mCellWidth * i, 0 };
    Point p2 = { mCellWidth * (1 + i), 0 };
    Point p3 = { mCellWidth * (0.5 + i), mCellHeight };

    switch (quad)
    {
    case 0:
        //Top Left:
        break;
	
    case 1:
        //Top Right:
        p1.x += 6 * mCellWidth + mBarWidth;
        p2.x += 6 * mCellWidth + mBarWidth;
        p3.x += 6 * mCellWidth + mBarWidth;
        break;

    case 2:
        //Bottom Right
        i = 5 - i;

        p1.x = mCellWidth * i;
        p2.x = mCellWidth * (1 + i);
        p3.x = mCellWidth * (0.5 + i);

        p1.x += 6 * mCellWidth + mBarWidth;
        p2.x += 6 * mCellWidth + mBarWidth;
        p3.x += 6 * mCellWidth + mBarWidth;

        p1.y = p2.y = mScreenHeight;
        p3.y = mScreenHeight - mCellHeight;
        break;

    case 3:
        //Bottom Left
        i = 5 - i;

        p1.x = mCellWidth * i;
        p2.x = mCellWidth * (1 + i);
        p3.x = mCellWidth * (0.5 + i);

        p1.y = p2.y = mScreenHeight;
        p3.y = mScreenHeight - mCellHeight;
        break;
    }

    // Drawing the triangle
    sf::Color currentColor(cellColor.r, cellColor.g, cellColor.b, 0xFF);

    sf::ConvexShape cellTriangle;
    cellTriangle.setPointCount(3);

    cellTriangle.setFillColor(currentColor);

    cellTriangle.setOutlineThickness(3);
    cellTriangle.setOutlineColor(sf::Color::Black);
    
    cellTriangle.setPoint(0, sf::Vector2f(p1.x, p1.y));
    cellTriangle.setPoint(1, sf::Vector2f(p2.x, p2.y));
    cellTriangle.setPoint(2, sf::Vector2f(p3.x, p3.y));

    mWindow->draw(cellTriangle);

    // Displaying the pieces
    for (int i = 0; i < c.tokenCount; i++)
    {
	if (i < 4)
	{
	    sf::Sprite token;
	    token.setTexture(mTexture[c.player]);

	    token.scale(sf::Vector2f((float)mTokenRadius / mSpriteRadius,
                                     (float)mTokenRadius / mSpriteRadius));

	    token.setOrigin(sf::Vector2f((float)mSpriteRadius, (float)mSpriteRadius));

	    if (p3.y < mScreenHeight / 2)
		token.setPosition(sf::Vector2f(p3.x, mTokenRadius + (2 * mTokenRadius + 2) * i));
	    else
		token.setPosition(sf::Vector2f(p3.x, (mScreenHeight - mTokenRadius) - (2 * mTokenRadius + 2) * i));

	    mWindow->draw(token);
	}
    }
}

float GUI_SFML::TriangleArea(int x1, int y1, int x2, int y2, int x3, int y3)
{
    return abs((x1*(y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

float GUI_SFML::TriangleArea(Point p1, Point p2, Point p3)
{
    return TriangleArea(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void GUI_SFML::PickupToken()
{    
    mPickupf = true;
}

void GUI_SFML::DropToken()
{
    mPickupf = false;
}
