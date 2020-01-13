#include "Utility.h"

Color::Color()
{
    r = g = b = 0;
}

// Decodes std::string hexadecimal color value
Color::Color(std::string hexCode)
{
    if (hexCode[0] == '#')
	hexCode = hexCode.substr(1, hexCode.size() - 1);

    for (auto& c : hexCode)
    {
	if (c >= 'A' && c <= 'Z')
	    c += 'z' - 'Z';
    }

    unsigned int rgb[3] = { 0 };

    for (int i = 0; i < 6; i += 2)
    {
	if (hexCode[i] >= '0' && hexCode[i] <= '9')
	    rgb[i / 2] += 16 * (hexCode[i] - '0');
	else
	    rgb[i / 2] += 16 * (hexCode[i] - 'a' + 10);

	if (hexCode[i + 1] >= '0' && hexCode[i + 1] <= '9')
	    rgb[i / 2] += (hexCode[i + 1] - '0');
	else
	    rgb[i / 2] += (hexCode[i + 1] - 'a' + 10);
    }

    r = rgb[0], g = rgb[1], b = rgb[2];
}

Color::Color(const char* hexCode)
{
    std::string hex = hexCode;
    *this = Color(hex);
}

// Decodes integer hexadecimal color code
Color::Color(int hexCode)
{
    r = 0;
    g = 0;
    b = 0;

    b += hexCode % 16;
    hexCode /= 16;

    b += 16 * (hexCode % 16);
    hexCode /= 16;

    g += hexCode % 16;
    hexCode /= 16;

    g += 16 * (hexCode % 16);
    hexCode /= 16;

    r += hexCode % 16;
    hexCode /= 16;

    r += 16 * (hexCode % 16);
    hexCode /= 16;
}

Cell::Cell(STATUS _status, STATUS _player, int _tokenCount) :
    status(_status), player(_player), tokenCount(_tokenCount)
{
}
