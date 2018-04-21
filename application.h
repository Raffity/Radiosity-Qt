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
    Radiosity *scene;
    bool dispFunc;
    QVector<Object> objects;
    QVector<colorMTL> colors;

    QList<QString> getObjects();
    Object *getObject(int numObj);           //возвращает указатель на выбранный объект из списка объектов
    bool loadFromOBJFile (QString filname);    //загрузка геометрии сцены из файла
    bool loadFromMTLFile (QString filename);    //загрузка текстур сцены из файла
    bool saveSceneToObj();
    void searchMTLtoOBJ();
    void preview();                 //отображение сцены в режиме превью

    bool init(QString filename,int h,int w);
    void reshapeFunc();
    void displayFunc();                         //отображение сцены после рендера
    void idleFunc(int maxIterations, int hemicubeResolution);

protected:
    int hemi=0;
    int width, height;
    int numIterations;
};

#endif // APPLICATION_H
