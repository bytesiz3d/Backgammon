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
    int tokenCount;

    Cell(STATUS _status = IDLE, int _tokenCount = 0);
};

class Color
{
public:
    int r, g, b;

    Color();
    Color(string hexCode);
    Color(const char* hexCode);
    Color(int hexCode);
};
