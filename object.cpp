#include "object.h"
#include <vector>
#include <QtGui>
#include <uvec3.h>
#include <vec3.h>
#include <QString>

using namespace std;

Object::Object()
{
    str = 3;
    color = vec3(0.4,0.4,0.4);
    emissionColor = vec3(0,0,0);
    vertices.clear();
    countVertex = 0;
    countFaces = 0;
}

void Object::setName(string Name)
{
    name=Name;
}

string Object::getName()
{
    return name;
}

QColor Object::getColor()
{
    QColor reColor;
    reColor.setRgbF(color.x,color.y,color.z);
    return reColor;
}

QColor Object::getEmissionColor()
{
    QColor reColor;
    reColor.setRgbF(emissionColor.x,emissionColor.y,emissionColor.z);
    return reColor;
}
vec3 Object::getColorV()
{
    return color;
}

vec3 Object::getEmissionColorV()
{
    return emissionColor;
}

int Object::getEmissionStrength()
{
    return str;
}

void Object::deleteVertices()
{
    vertices.clear();
}

void Object::loadMovedPoint(int numPoint,vec3 point)
{
    vertices[numPoint] = point ;
}

void Object::loadPoint(vec3 point)
{
    vertices.push_back(point);
    countVertex++;
}

void Object::loadFace(uvec3 face)
{
    faces.push_back(face);
    countFaces++;
}

void Object::setEmissionColor(vec3 pcolor)
{
    emissionColor = pcolor;
}

void Object::setEmissionColor(QColor pcolor,int strength)
{
    emissionColor.x = pcolor.redF();
    emissionColor.y = pcolor.greenF();
    emissionColor.z = pcolor.blueF();
    str = strength;
    emissionColor = emissionColor * strength;
}

void Object::setColor(QColor pcolor)
{
    color.x=pcolor.redF();
    color.y=pcolor.greenF();
    color.z=pcolor.blueF();
}

void Object::setColor(vec3 pcolor)
{
    color=pcolor;
}
std::vector<vec3> Object:: getVertices()
{
    return vertices;
}

std::vector<uvec3> Object:: getFaces()
{
    return faces;
}
