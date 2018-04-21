#ifndef COLOR_H
#define COLOR_H
#include <vec3.h>
#include <QString>

using namespace std;

class Color
{
public:
    Color();
    string name;
    vec3 color;
    vec3 emissionColor;
};

#endif // COLOR_H
