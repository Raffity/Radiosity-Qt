#include "application.h"
#include "Radiosity.h"
#include "Camera.h"
#include <GL/glu.h>
#include <GL/gl.h>
#include <QMessageBox>
#include <QString>
#include <object.h>
#include <vec3.h>
#include <fstream>
#include <sstream>
#include <uvec3.h>
#include <QIODevice>
#include <QFile>
#include <algorithm>


Application::Application()
{
    hemi=0;
    numIterations = 0;
    camera.position = vec3(15, 20, 15);
    camera.pitch = -20;
    camera.yaw = 45;
    dispFunc=false;
    objects.clear();
    scene = NULL;
}

//метод отрисовки всех объектов
void Application::preview()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK, style);
    camera.applyTransformation();
    glViewport(0, 0, width, height);
   for (unsigned int fi = 0; fi < previewFaces.size(); fi++) {
       uvec3& face =  previewFaces[fi];
       vec3& v1 = previewPoints[face.x];
       vec3& v2 = previewPoints[face.y];
       vec3& v3 = previewPoints[face.z];

       vec3& c1 = previewColors[face.x];
       vec3& c2 = previewColors[face.y];
       vec3& c3 = previewColors[face.z];

       glBegin(GL_TRIANGLES);
           glColor3f(c1.x, c1.y, c1.z);
           glVertex3f(v1.x, v1.y, v1.z);
           glColor3f(c2.x, c2.y, c2.z);
           glVertex3f(v2.x, v2.y, v2.z);
           glColor3f(c3.x, c3.y, c3.z);
           glVertex3f(v3.x, v3.y, v3.z);
       glEnd();
    }
   glDisable(GL_LINE_SMOOTH);
   glFinish();
}


void Application::previewCreator()
{
    double vCount = 0;
    previewPoints.clear();
    previewColors.clear();
    previewFaces.clear();
    for(int n=0;n<objects.size();n++)
    {
        for(int i=0;i<objects[n].countVertex;i++)
        {
            previewPoints.push_back(objects[n].getVertices()[i]);
            if(objects[n].selected)
            {
                previewColors.push_back(vec3(0,1,0));
            }
            else
            {
                previewColors.push_back(objects[n].getColorV());
            }
        }
        for(int i=0;i<objects[n].countFaces;i++)
        {
            previewFaces.push_back(uvec3(objects[n].getFaces()[i].x + vCount,
                                  objects[n].getFaces()[i].y + vCount,
                                  objects[n].getFaces()[i].z + vCount));
        }
        vCount = previewPoints.size();
    }
}

//метод возвращает список имен объектов
QList<QString> Application::getObjects()
{
    QList<QString> listObj;
    for(int i=0;i<objects.size();i++)
    {
        listObj.push_back(QString::fromStdString(objects[i].getName()));
    }
    return listObj;
}


Object *Application::getObject(int numObj)
{
    return &objects[numObj];
}


//метод добавления нового объекта на сцену
void Application::addExistingObj(QString nameObj)
{
    ifstream fs;
    QString str = QString("scene_creator.obj");
    fs.open(str.toUtf8());
    char c;
    vec3 vertex;
    uvec3 face;
    int index[3];
    std::string line="";
    Object *obj=new Object();
    string name="";
    QString qName;
    int vertexObj =0;
    int firstVertex =0;
    bool first = true;
    while (std::getline(fs, line)) {
        std::istringstream iss(line);
        iss >> c;
        switch (c) {
        case 'o':
            iss >> name;
            qName = QString::fromStdString(name);
            if(qName != nameObj)
            {
                continue;
            }
            obj=new Object();
            obj->setName(name + "_" + std::to_string(objects.size()));
            obj->mtl = nameObj.toUtf8().constData() + std::to_string(objects.size());
            break;
        case 'v':
            if(qName != nameObj)
            {
                continue;
            }
            iss >> vertex.x;
            iss >> vertex.y;
            iss >> vertex.z;
            totalVertex++;
            vertexObj++;
            obj->loadPoint(vertex);
            break;
        case 'f':
            if(qName != nameObj)
            {
                continue;
            }
            iss >> index[0];
            iss >> index[1];
            iss >> index[2];
            if(first)
            {
                firstVertex = min(index[0],min(index[1],index[2]));
                first = false;
            }
            face = uvec3(index[0] - firstVertex,
                      index[1] - firstVertex,
                      index[2] - firstVertex);
            totalFaces++;
            obj->loadFace(face);
            break;
        default:
            break;
        }
    }
    objects.push_back(*obj);
    fs.close();
}

//загрузка сцены формата obj
//filename путь и имя файла
bool Application::loadFromOBJFile(QString filename) {
    objects.clear();
    ifstream fs;
    fs.open(filename.toLocal8Bit().data());
    char c;
    vec3 vertex;
    uvec3 face;
    int index[3];
    std::string line="";
    if(filename.isNull())
    {
        return false;
    }
    Object *obj=new Object();
    string name="";
    string mtl;
    bool first=true;
    int numVertex=0;
    while (std::getline(fs, line)) {
        std::istringstream iss(line);
        iss >> c;
        switch (c) {
        case 'o':
            if(name!=""){
                objects.push_back(*obj);
                first=false;
            }
            obj=new Object();
            iss >> name;
            obj->setName(name);
            break;
        case 'v':
            iss >> vertex.x;
            iss >> vertex.y;
            iss >> vertex.z;
            if(iss >>vertex.x)
            {
                return false;
            }
            totalVertex++;
            numVertex++;
            obj->loadPoint(vertex);
            break;
        case 'u':
            iss >> c;
            iss >> c;
            iss >> c;
            iss >> c;
            iss >> c;
            iss>>mtl;
            obj->mtl=mtl;
            break;
        case 'f':
            iss >> index[0];
            iss >> index[1];
            iss >> index[2];
            if(first)
            {
                face=uvec3(index[0] - 1, index[1] -1, index[2] -1);
            }
            else{
                face = uvec3(index[0] - 1 - numVertex + obj->countVertex,
                        index[1] -1-numVertex + obj->countVertex,
                        index[2] -1- numVertex + obj->countVertex);
            }
            totalFaces++;
            obj->loadFace(face);
            break;
        default:
            break;
        }
    }
    objects.push_back(*obj);
    fs.close();
    return true;
}

//парсер файла MTL
bool Application::loadFromMTLFile (QString filename)
{
    ifstream fs;
    fs.open(filename.toLocal8Bit().data());
    char c;
    vec3 color;
    vec3 emissionColor;
    colorMTL *col=new colorMTL();
    std::string line="";
    if(filename.isNull())
    {
        return false;
    }
    string name="";
    while (std::getline(fs, line))
    {
        std::istringstream iss(line);
        iss >> c;
        switch (c) {
        case 'n':
            iss >> c;
            iss >> c;
            iss >> c;
            iss >> c;
            iss >> c;
            if(name!="")
            {
                colors.push_back(*col);
            }
             iss >> name;
             col->name=name;
            break;
        case 'K':
        {
            iss >> c;
            switch(c)
            {
            case 'd':
            { iss>>color.x;
                iss>>color.y;
                iss>>color.z;
                col->color=color;
                break;
            }
            case 'e':
            {
                iss>>emissionColor.x;
                iss>>emissionColor.y;
                iss>>emissionColor.z;
                col->emissionColor=emissionColor;
                break;
            }
            default:
                break;
            }
        }
        default:
            break;
        }
    }
    colors.push_back(*col);
    fs.close();
    return true;
}

void Application::searchMTLtoOBJ()
{
    for(int i=0;i<objects.size();i++)
    {
        for(int j=0;j<colors.size();j++)
        {
            if(objects[i].mtl==colors[j].name)
            {
                objects[i].setColor(colors[j].color);
                objects[i].setEmissionColor(colors[j].emissionColor);
                continue;
            }
        }
    }
}



bool Application::saveSceneToObj()
{
    double vertCount = 0;
    QFile sceneObj("scene_radiosity.obj");
    QFile sceneMTL("scene_radiosity.mtl");
    if(sceneObj.open(QIODevice::WriteOnly | QFile::Text))
    {
        QTextStream out(&sceneObj);
        out.setCodec("UTF-8");
        out<<"#Radiosity_QT\n";
        out<<"mtllib scene_radiosity.mtl\n";
        for(int i=0;i<objects.size();i++)
        {
            out<< "o " + QString::fromStdString(objects[i].getName()) + "\n";
            for(int vert=0;vert<objects[i].getVertices().size();vert++)
            {
                out<<"v "+QString::number(objects[i].getVertices()[vert].x)+" "
                     + QString::number(objects[i].getVertices()[vert].y)+" "
                     + QString::number(objects[i].getVertices()[vert].z)+"\n";
            }
            out<<"usemtl " + QString::fromStdString(objects[i].mtl)+"\n";
            out<<"s off\n";
            for(int face=0; face<objects[i].getFaces().size();face++)
            {
                out<<"f "+QString::number(objects[i].getFaces()[face].x+1+vertCount)+" "
                     + QString::number(objects[i].getFaces()[face].y+1+vertCount)+" "
                     + QString::number(objects[i].getFaces()[face].z+1+vertCount)+"\n";
            }
            vertCount += objects[i].countVertex;
        }
    }
    if(sceneMTL.open(QIODevice::WriteOnly))
    {
          QTextStream out(&sceneMTL);
          out<<"Radiosity_QT\n";
          for(int i=0;i<colors.size();i++)
          {
            out<<"newmtl "+QString::fromStdString(colors[i].name)+"\n";
            out<<"Ns 0\n";
            out<<"Ka 0 0 0\n";
            out<<"Kd "+QString::number(colors[i].color.x)+" "
                    +QString::number(colors[i].color.y)+" "
                 +QString::number(colors[i].color.z)+"\n";
            out<<"Ks 0 0 0\n";
            out<<"Ke "+QString::number(colors[i].emissionColor.x)+" "
                    +QString::number(colors[i].emissionColor.y)+" "
                 +QString::number(colors[i].emissionColor.z)+"\n";
            out<<"Ni 0.000000\n";
            out<<"d 0.000000\n";
            out<<"illum 1\n\n";
          }
    }
}

bool Application::init(QString filename) {
    if(!loadFromOBJFile(filename) && filename != NULL)
    {
       return false;
    }
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0,0,0,0);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-24, 24, -24, 24, 0.01, 10000);
    glMatrixMode(GL_MODELVIEW);
    return true;
}

void Application::reshapeFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    camera.applyTransformation();
    scene->render();
    glFinish();
}

void Application::displayFunc() {
    if (scene != NULL)
    {
        delete scene;
    }
    scene = new Radiosity();
    dispFunc=true;
    for(int i=0;i<objects.size();i++)
    {
        scene->assembly(&objects[i]);
    }
    scene->buildVertexToFaceMap();
    scene->autoCalculateNormals();
}


void Application::idleFunc(int maxIterarions,int hemicubeResolution) {
    if(hemi!=hemicubeResolution)
    {
        scene->calculateFormFactorsHemicube(hemicubeResolution);
    }
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-24, 24, -24, 24, 0.01, 10000);
    glMatrixMode(GL_MODELVIEW);
    while(numIterations < maxIterarions)
    {
        scene->calculateRadiosities();
        scene->interpolateColors();
        numIterations++;
    }
    hemi=hemicubeResolution;
}
