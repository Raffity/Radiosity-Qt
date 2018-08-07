#ifndef APPLICATION_H
#define APPLICATION_H
#include "Radiosity.h"
#include "Camera.h"
#include <GL/glu.h>
#include <GL/gl.h>
#include <QString>
#include <colormtl.h>
#include <object.h>

class Application
{
public:
    GLenum style = GL_LINE;
    Application();

    Camera camera;
    bool dispFunc = false;
    int totalVertex = 0;
    int totalFaces = 0;
    int width, height;
    QVector<Object> objects;
    QVector<colorMTL> colors;

    QList<QString> getObjects();
    Object *getObject(int numObj);           //возвращает указатель на выбранный объект из списка объектов
    bool loadFromOBJFile (QString filname);    //загрузка геометрии сцены из файла
    bool loadFromMTLFile (QString filename);    //загрузка текстур сцены из файла
    bool saveSceneToObj();                      //сохранение сцены в формате obj
    void searchMTLtoOBJ();
    void preview();                         //отображение сцены в режиме превью
    void addExistingObj(QString nameObj);   //добавление нового объекта на сцену

    bool init(QString filename);                //установка параметров отображения
    void reshapeFunc();                         //перерисовка отрендеренной сцены
    void displayFunc();                         //отображение сцены после рендера
    void idleFunc(int maxIterations, int hemicubeResolution); //расчет форм-факторов и излучательности
    void previewCreator();

private:
    Radiosity *scene = NULL;
    int hemi=0;
    int numIterations;
    vector<vec3> previewPoints;
    vector<vec3> previewColors;
    vector<uvec3> previewFaces;
};

#endif // APPLICATION_H
