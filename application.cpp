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


Application::Application()
{
    hemi=0;
    numIterations = 0;
    camera.position = vec3(15, 20, 15);
    camera.pitch = -20;
    camera.yaw = 45;
    dispFunc=false;
    scene=new Radiosity();
    objects.clear();
}

//метод отрисовки всех объектов
void Application::preview()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK, style);
    camera.applyTransformation();
    vec3 c1;
    vec3 c2;
    vec3 c3;
    for(unsigned int i=0;i<objects.size();i++)
    {
    for (unsigned int fi = 0; fi < objects[i].getPreFaces().size(); fi++) {
        uvec3 face =  objects[i].getPreFaces()[fi];
        vec3 v1 = objects[i].getVertices()[face.x];
        vec3 v2 = objects[i].getVertices()[face.y];
        vec3 v3 = objects[i].getVertices()[face.z];
        if(objects[i].selected)
        {
            c1=vec3(0,1,0);
            c2=vec3(0,1,0);
            c3=vec3(0,1,0);
        }
        else{
             c1 = objects[i].getVertexColors()[face.x];
             c2 = objects[i].getVertexColors()[face.y];
             c3 = objects[i].getVertexColors()[face.z];
        }
        glBegin(GL_TRIANGLES);
            glColor3f(c1.x, c1.y, c1.z);
            glVertex3f(v1.x, v1.y, v1.z);
            glColor3f(c2.x, c2.y, c2.z);
            glVertex3f(v2.x, v2.y, v2.z);
            glColor3f(c3.x, c3.y, c3.z);
            glVertex3f(v3.x, v3.y, v3.z);
        glEnd();
        }
    }
   glFinish();
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

bool Application::loadFromOBJFile(QString filename) {
    objects.clear();
    ifstream fs;
    fs.open(filename.toUtf8());
    char c;
    vec3 vertex;
    uvec3 face;
    uvec3 preFace;
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
                preFace=uvec3(index[0] - 1, index[1] -1, index[2] -1);
            }
            else{
                preFace = uvec3(index[0] - 1 - numVertex + obj->countVertex,
                        index[1] -1-numVertex + obj->countVertex,
                        index[2] -1- numVertex + obj->countVertex);
            }
            face = uvec3(index[0] - 1, index[1] -1, index[2] -1);
            obj->loadFace(face);
            obj->loadPreFace(preFace);
            break;
        default:
            break;
        }
    }
    objects.push_back(*obj);
    fs.close();
    return true;
}

bool Application::loadFromMTLFile (QString filename)
{
    ifstream fs;
    fs.open(filename.toUtf8());
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

    QFile sceneObj("scene.obj");
    QFile sceneMTL("scene.mtl");
    if(sceneObj.open(QIODevice::WriteOnly))
    {
        QTextStream out(&sceneObj);
        out<<"mtllib scene.mtl\n";
        for(int i=0;i<objects.size();i++)
        {
            out<<"o "+QString::fromStdString(objects[i].getName())+"\n";
            for(int vert=0;vert<objects[i].getVertices().size();vert++)
            {
                out<<"v "+QString::number(objects[i].getVertices()[vert].x)+" "
                     + QString::number(objects[i].getVertices()[vert].y)+" "
                     + QString::number(objects[i].getVertices()[vert].z)+"\n";
            }
            out<<"usemtl " + QString::fromStdString(objects[i].mtl)+"\n";
            out<<"s off\n";
            for(int face=0;face<objects[i].getFaces().size();face++)
            {
                out<<"f "+QString::number(objects[i].getFaces()[face].x+1)+" "
                     + QString::number(objects[i].getFaces()[face].y+1)+" "
                     + QString::number(objects[i].getFaces()[face].z+1)+"\n";
            }
        }
    }
    if(sceneMTL.open(QIODevice::WriteOnly))
    {
          QTextStream out(&sceneMTL);
          for(int i=0;i<colors.size();i++)
          {
            out<<"newmtl "+QString::fromStdString(colors[i].name)+"\n";
            out<<"Ns 0\n";
            out<<"Ka 0 0 0\n";
            out<<"Kd"+QString::number(colors[i].color.x)+" "
                    +QString::number(colors[i].color.y)+" "
                 +QString::number(colors[i].color.z)+"\n";
            out<<"Ks 0,5 0,5 0,5\n";
            out<<"Ke"+QString::number(colors[i].emissionColor.x)+" "
                    +QString::number(colors[i].emissionColor.y)+" "
                 +QString::number(colors[i].emissionColor.z)+"\n";
            out<<"Ni 1.000000\n";
            out<<"d 1.000000\n";
            out<<"illum 2\n";
          }
    }
}

bool Application::init(QString filename,int h,int w) {
   if(!loadFromOBJFile(filename))
   {
       return false;
   }
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0,0,0,0);
    height=h;
    width=w;
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
        scene->calculateFormFactors(hemicubeResolution);
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
    dispFunc=true;

}
