#ifndef COLORMTL_H
#define COLORMTL_H

#include <vec3.h>
#include <QString>

using namespace std;

class colorMTL
{
public:
    colorMTL();
    string name;
    vec3 color;
    vec3 emissionColor;
};

#endif // COLORMTL_H
