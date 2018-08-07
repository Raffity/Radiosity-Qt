#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <QtGui>
#include <uvec3.h>
#include <vec3.h>
#include <QString>


using namespace std;

class Object
{
public:
    Object();
    unsigned int countVertex=0; //число вершин объекта
    unsigned int countFaces=0; //число патчей объекта
    bool selected = false;
    string mtl;

    string getName();
    void setName(string Name);
    void loadPoint(vec3 point);
    void loadMovedPoint(int numPoint,vec3 point);
    void loadFace(uvec3 face);

    void setColor(QColor pcolor);     //Устанавливает цвет объекта
    void setColor(vec3 pcolor);
    void setEmissionColor(QColor pcolor,int strength);    //Устанавливает излучающий цвет объекта
    void setEmissionColor(vec3 pcolor);
    void deleteVertices();

    std::vector<vec3> getVertices();
    std::vector<uvec3> getFaces();
    QColor getColor();
    QColor getEmissionColor();
    vec3 getColorV();
    vec3 getEmissionColorV();
    int getEmissionStrength();

protected:

    int str;
    vec3 color;          //цвет объекта
    vec3 emissionColor; //излучающий цвет объекта
    string name;        //имя объекта
    std::vector<vec3> vertices; //вектор вершин
    std::vector<uvec3> faces; //вектор с номерами вершин для каждого объекта
};

#endif // OBJECT_H
