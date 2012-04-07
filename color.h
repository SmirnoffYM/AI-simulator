#ifndef COLOR_H
#define COLOR_H

#include <string>

class Color
{
public:
    Color(unsigned int red, unsigned int green, unsigned int blue);
    //TODO: implement constructor receiving std::string as single parameter

    std::string toString();
private:
    int r;
    int g;
    int b;
public:
    int red()
    {
        return r;
    }

    int green()
    {
        return g;
    }

    int blue()
    {
        return b;
    }
};

#endif // COLOR_H
