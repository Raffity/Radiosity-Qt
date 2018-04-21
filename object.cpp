#include "object.h"
#include <vector>
#include <QtGui>
#include <uvec3.h>
#include <vec3.h>
#include <QString>

using namespace std;

Object::Object()
{
    str=2;
    color=vec3(0.4,0.4,0.4);
    emissionColor=vec3(0,0,0);
    vertices.clear();
    faces.clear();
    vertexColors.clear();
    faceEmissions.clear();
    faceReflectivities.clear();
    countVertex=0;
    countFaces=0;
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
    reColor.setRgbF(emissionColor.x/str,emissionColor.y/str,emissionColor.z/str);
    return reColor;
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
    vertexColors.push_back(color);
    countVertex++;
}

void Object::loadFace(uvec3 face)
{
    faces.push_back(face);
    faceEmissions.push_back(emissionColor);
    faceReflectivities.push_back(color);
    countFaces++;
}
void Object::loadPreFace(uvec3 face)
{
    preFaces.push_back(face);
}

void Object::setEmissionColor(vec3 pcolor)
{
    emissionColor=pcolor;
    faceEmissions.clear();
    for(unsigned int i=0; i<countFaces; i++)
    {
        faceEmissions.push_back(emissionColor);
    }
}
void Object::setColor(vec3 pcolor)
{
    color=pcolor;
    vertexColors.clear();
    faceReflectivities.clear();
    for(unsigned int i=0; i<countVertex; i++)
    {
        vertexColors.push_back(color);
    }
    for(unsigned int i=0; i<countFaces; i++)
    {
        faceReflectivities.push_back(color);

    }
}
void Object::setEmissionColor(QColor pcolor,int strength)
{
    emissionColor.x=pcolor.redF();
    emissionColor.y=pcolor.greenF();
    emissionColor.z=pcolor.blueF();
    faceEmissions.clear();
    str=strength;
    emissionColor=emissionColor*strength;
    for(unsigned int i=0; i<countFaces; i++)
    {
        faceEmissions.push_back(emissionColor);
    }
}
void Object::setColor(QColor pcolor)
{
    color.x=pcolor.redF();
    color.y=pcolor.greenF();
    color.z=pcolor.blueF();
    vertexColors.clear();
    faceReflectivities.clear();
    for(unsigned int i=0; i<countVertex; i++)
    {
        vertexColors.push_back(color);
    }
    for(unsigned int i=0; i<countFaces; i++)
    {
        faceReflectivities.push_back(color);
    }
}


std::vector<vec3> Object:: getVertices()
{
    return vertices;
}
std::vector<vec3> Object:: getVertexColors()
{
    return vertexColors;
}
std::vector<uvec3> Object:: getFaces()
{
    return faces;
}
std::vector<uvec3> Object:: getPreFaces()
{
    return preFaces;
}
std::vector<vec3> Object:: getFaceEmissions()
{
    return faceEmissions;
}
std::vector<vec3> Object:: getFaceReflectivities()
{
    return faceReflectivities;
}
