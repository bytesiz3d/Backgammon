#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <vector>
using namespace std;

struct Point
{
    int x, y;
};

enum STATUS
{
    WHITE,
    BLACK,
    IDLE,
    HIGHLIGHTED,
    SELECTED,
    STATUS_COUNT
};

enum PROGRAM_MODE
{
    STANDBY,
    PIECE_SELECTION,
    MOVE_SELECTION,
    BEARING_OFF,
    BO_MOVE_SELECTION,
    MODE_COUNT
};

class Cell
{
public:
    STATUS status;
    STATUS player;
    int tokenCount;

    Cell(STATUS _status = IDLE, STATUS _player = IDLE, int _tokenCount = 0);
};

class Color
{
public:
    unsigned int r, g, b;

    Color();
    Color(string hexCode);
    Color(const char* hexCode);
    Color(unsigned int hexCode);
};
