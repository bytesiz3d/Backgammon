#include "GUI_SFML.h"
#include "Roboto-Medium-ttf.h"
#include <imgui-SFML.h>
#include <imgui.h>

int GUI_SFML::mX = 0;
int GUI_SFML::mY = 0;

bool GUI_SFML::mClosef = false;

bool GUI_SFML::mLeftClickf = false;
bool GUI_SFML::mCancelf = false;

bool GUI_SFML::mSpacePressf = false;
bool GUI_SFML::mPickupf = false;

GUI_SFML::GUI_SFML()
	: mScreenWidth(1280), mScreenHeight(900), mCellWidth(100), mCellHeight(360),
	  mTokenRadius(45), mBarWidth(80),
	  mSpriteRadius(45),
	  mWindow(sf::VideoMode(mScreenWidth, mScreenHeight), "Backgammon!", sf::Style::Titlebar | sf::Style::Close, sf::ContextSettings(0, 0, 8)), mDeltaClock(),
	  mFont()
{
	srand(time(0));
	mFont.loadFromMemory(Roboto_Medium_ttf, Roboto_Medium_ttf_len);

	ImGui::SFML::Init(mWindow);

	// Setting the color environment:
	mColors[HIGHLIGHTED] = "#B4D5FF";
	mColors[IDLE] = "#381704";
	mColors[SELECTED] = "#00FF00";
	mColors[BLACK] = mColors[IDLE];
	mColors[WHITE] = mColors[IDLE];

	// Loading the textures:
	if (!mTexture[WHITE].loadFromFile(ASSET_DIR "/white-90.png") ||
		!mTexture[BLACK].loadFromFile(ASSET_DIR "/black-90.png"))
	{
		std::cerr << "Failed to load textures\n";
		mClosef = true;
	}
}

int
GUI_SFML::CellNumber()
{
	// P (mX, mY)
	Point P = {mX, mY};

	float At, A1, A2, A3;

	sf::Sprite token;
	token.setTexture(mTexture[0]);
	token.setOrigin(sf::Vector2f((float)mSpriteRadius, (float)mSpriteRadius));

	for (int i = 0; i < 6; i++)
	{
		//Top Left:
		Point A = {mCellWidth * i, 0};
		Point B = {mCellWidth * (1 + i), 0};
		Point C = {mCellWidth * (0.5 + i), mCellHeight};

		At = TriangleArea(A, B, C);
		A1 = TriangleArea(P, B, C);
		A2 = TriangleArea(P, A, C);
		A3 = TriangleArea(P, A, B);

		if (At == A1 + A2 + A3)
			return i;
		else
		{
			// Check for collision with the final circle:
			token.setPosition(sf::Vector2f(C.x, mTokenRadius + (2 * mTokenRadius + 2) * 3));

			if (token.getGlobalBounds().contains(P.x, P.y))
				return i;
		}

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
		else
		{
			// Check for collision with the final circle:
			token.setPosition(sf::Vector2f(C.x, mTokenRadius + (2 * mTokenRadius + 2) * 3));

			if (token.getGlobalBounds().contains(P.x, P.y))
				return i + 6;
		}

		//Bottom Right:
		A.y = B.y = mScreenHeight;
		C.y = mScreenHeight - mCellHeight;

		At = TriangleArea(A, B, C);
		A1 = TriangleArea(P, B, C);
		A2 = TriangleArea(P, A, C);
		A3 = TriangleArea(P, A, B);

		if (At == A1 + A2 + A3)
			return 17 - i;
		else
		{
			// Check for collision with the final circle:
			token.setPosition(sf::Vector2f(C.x, (mScreenHeight - mTokenRadius) - (2 * mTokenRadius + 2) * 3));

			if (token.getGlobalBounds().contains(P.x, P.y))
				return 17 - i;
		}

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
		else
		{
			// Check for collision with the final circle:
			token.setPosition(sf::Vector2f(C.x, (mScreenHeight - mTokenRadius) - (2 * mTokenRadius + 2) * 3));

			if (token.getGlobalBounds().contains(P.x, P.y))
				return 23 - i;
		}
	}

	return -1;
}

void
GUI_SFML::PollEvents()
{
	// Updating the mouse position:
	mX = sf::Mouse::getPosition(mWindow).x;
	mY = sf::Mouse::getPosition(mWindow).y;

	sf::Event ev;

	while (mWindow.pollEvent(ev))
	{
		ImGui::SFML::ProcessEvent(ev);
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

	ImGui::SFML::Update(mWindow, mDeltaClock.restart());
}

void
GUI_SFML::UpdateInteface(PROGRAM_MODE CURRENT_MODE, std::span<int> moves, Cell *board, STATUS PLAYER)
{
	mLeftClickf = false;
	mCancelf = false;
	mSpacePressf = false;

	// Draw:
	mWindow.clear();
	DrawBoard(CURRENT_MODE, board);

	// Handling the token:
	if (mPickupf)
	{
		sf::Sprite token;
		token.setTexture(mTexture[PLAYER]);

		token.setOrigin(sf::Vector2f((float)mSpriteRadius, (float)mSpriteRadius));

		token.setPosition(sf::Vector2f(sf::Mouse::getPosition(mWindow).x, sf::Mouse::getPosition(mWindow).y));

		mWindow.draw(token);
	}

	ImGui::SetNextWindowSize({200, 0}, ImGuiCond_Appearing);
	if (ImGui::Begin("Game"))
	{
		constexpr const char *PLAYER_NAMES[STATUS_COUNT] = {"WHITE", "BLACK"};
		constexpr const char *MODE_NAMES[MODE_COUNT] = {
			"STANDBY",
			"PIECE_SELECTION",
			"MOVE_SELECTION",
			"BEARING_OFF",
			"BO_MOVE_SELECTION",
		};

		ImGui::Text("SPACE: Roll dice\n"
					"LMB: Pickup piece\n"
					"RMB: Cancel piece pickup");
		ImGui::Separator();

		switch (CURRENT_MODE)
		{
		case STANDBY:
			ImGui::Text("Roll the dice!");
			ImGui::Separator();
			break;

		case PIECE_SELECTION: {
			ImGui::Text("Moves: ");
			for (auto m: moves)
			{
				ImGui::SameLine();
				ImGui::Text("%d", m);
			}
			ImGui::Separator();
			break;
		}

		default:
			break;
		}

		if (ImGui::BeginTable("##stats", 2, ImGuiTableFlags_BordersInnerV, {-FLT_MIN, -FLT_MIN}))
		{
			ImGui::TableSetupColumn("##labels", ImGuiTableColumnFlags_WidthFixed);

			ImGui::TableNextColumn(); ImGui::Text("Player");
			ImGui::TableNextColumn(); ImGui::Text(PLAYER_NAMES[PLAYER]);

			ImGui::TableNextColumn(); ImGui::Text("Mode");
			ImGui::TableNextColumn(); ImGui::Text(MODE_NAMES[CURRENT_MODE]);

			if (mPickupf)
			{
				ImGui::TableNextColumn(); ImGui::Text("Token");
				ImGui::TableNextColumn(); ImGui::Text("HELD");
			}

			ImGui::EndTable();
		}
	}
	ImGui::End();

	// Swap Buffers:
	ImGui::SFML::Render(mWindow);
	mWindow.display();
}

bool
GUI_SFML::Clicked()
{
	return mLeftClickf;
}

bool
GUI_SFML::Cancelled()
{
	return mCancelf;
}

bool
GUI_SFML::Rolled()
{
	return mSpacePressf;
}

bool
GUI_SFML::Closed()
{
	return mClosef;
}

GUI_SFML::~GUI_SFML()
{
}

void
GUI_SFML::DrawBoard(PROGRAM_MODE CURRENT_MODE, Cell *board)
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
	mWindow.clear(currentColor);

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

	mWindow.draw(middleBar);

	sf::RectangleShape middleLine(sf::Vector2f(4, mScreenHeight));
	middleLine.setPosition(mScreenWidth / 2 - 2, 0);
	middleLine.setFillColor(sf::Color::Black);

	mWindow.draw(middleLine);
}

void
GUI_SFML::DrawCell(int i, const Cell &c)
{
	int quad = i / 6;
	Color cellColor = mColors[c.status];

	i %= 6;

	// Locating the three points
	Point p1 = {mCellWidth * i, 0};
	Point p2 = {mCellWidth * (1 + i), 0};
	Point p3 = {mCellWidth * (0.5 + i), mCellHeight};

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

	mWindow.draw(cellTriangle);

	// Displaying the pieces
	for (int i = 0; i < c.tokenCount; i++)
	{
		if (i < 4)
		{
			sf::Sprite token;
			token.setTexture(mTexture[c.player]);

			token.setOrigin(sf::Vector2f((float)mSpriteRadius, (float)mSpriteRadius));

			if (p3.y < mScreenHeight / 2)
				token.setPosition(sf::Vector2f(p3.x, mTokenRadius + (2 * mTokenRadius + 2) * i));
			else
				token.setPosition(sf::Vector2f(p3.x, (mScreenHeight - mTokenRadius) - (2 * mTokenRadius + 2) * i));

			mWindow.draw(token);
		}
		else
		{
			constexpr unsigned int textSize = 20;
			sf::Text text{std::to_string(c.tokenCount), mFont, textSize};
			text.setOrigin(sf::Vector2f(textSize / 2.f, textSize / 2.f));
			text.setColor(sf::Color::Black);

			float text_y = p3.y;
			if (p3.y < mScreenHeight / 2) // above
				text_y += 2 * textSize;
			else
				text_y -= 2 * textSize;

			text.setPosition(p3.x, text_y);
			mWindow.draw(text);
			break;
		}
	}
}

float
GUI_SFML::TriangleArea(int x1, int y1, int x2, int y2, int x3, int y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}

float
GUI_SFML::TriangleArea(Point p1, Point p2, Point p3)
{
	return TriangleArea(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
}

void
GUI_SFML::PickupToken()
{
	mPickupf = true;
}

void
GUI_SFML::DropToken()
{
	mPickupf = false;
}
