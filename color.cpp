#include "color.h"
#include <stdio.h>

Color::Color(unsigned int red, unsigned int green, unsigned int blue)
{
    (red <= 255) ? (r = red) : (r = 255);
    (green <= 255) ? (g = green) : (g = 255);
    (blue <= 255) ? (b = blue) : (b = 255);
}

std::string Color::toString()
{
    char* str = new char[7];
    sprintf(str, "#%02x%02x%02x", r, g, b);
    return std::string(str);
}
