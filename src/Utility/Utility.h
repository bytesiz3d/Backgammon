#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

// 2D pixel position
struct Point
{
	int x, y;
};

// Cell state
enum STATUS
{
	WHITE,
	BLACK,
	IDLE,
	HIGHLIGHTED,
	SELECTED,
	STATUS_COUNT
};

// Program state
enum PROGRAM_MODE
{
	STANDBY,
	PIECE_SELECTION,
	MOVE_SELECTION,
	BEARING_OFF,
	BO_MOVE_SELECTION,
	MODE_COUNT
};

// Cell handling
class Cell
{
public:
	STATUS status;
	STATUS player;
	int tokenCount;

	Cell(STATUS _status = IDLE, STATUS _player = IDLE, int _tokenCount = 0);
};

// Color handling
class Color
{
public:
	unsigned int r, g, b;

	Color();
	Color(std::string hexCode);
	Color(const char *hexCode);
	Color(int hexCode);
};
