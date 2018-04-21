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
    bool selected=false;
    string mtl;

    string getName();   //Возвращает имя объекта
    void setName(string Name);      //Устанавливает имя объекта и очищает все данные
    void loadPoint(vec3 point);     //Загружает одну точку объекта
    void loadMovedPoint(int numPoint,vec3 point);
    void loadFace(uvec3 face);      //Загружает один патч
    void loadPreFace(uvec3 face);

    void setColor(QColor pcolor);     //Устанавливает цвет объекта
    void setEmissionColor(QColor pcolor,int strength);    //Устанавливает излучающий цвет объекта
    void setColor(vec3 pcolor);     //Устанавливает цвет объекта
    void setEmissionColor(vec3 pcolor);    //Устанавливает излучающий цвет объекта
    void deleteVertices();

    std::vector<vec3> getVertices();
    std::vector<vec3> getVertexColors();
    std::vector<uvec3> getFaces();
    std::vector<uvec3> getPreFaces();
    std::vector<vec3> getFaceEmissions();
    std::vector<vec3> getFaceReflectivities();

    QColor getColor();        //Возвращает цвет объекта
    QColor getEmissionColor();   //Возвращает излучающий цвет объекта

protected:

    int str;
    string name;   //имя объекта
    vec3 color;    //цвет объекта
    vec3 emissionColor; //излучающий цвет объекта
    std::vector<vec3> vertices; //вектор вершин
    std::vector<vec3> vertexColors; //цвет вершины
    std::vector<uvec3> faces;   //вектор номеров вершин, образующих патч
    std::vector<uvec3> preFaces;   //вектор с номерами вершин для каждого объекта
    std::vector<vec3> faceEmissions;    //излучающий цвет поверхности
    std::vector<vec3> faceReflectivities;   //цвет поверхности
};

#endif // OBJECT_H
