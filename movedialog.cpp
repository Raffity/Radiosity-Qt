#include "movedialog.h"
#include "ui_movedialog.h"
#include <QString>
#include <math.h>


using namespace std;

moveDialog::moveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::moveDialog)
{
    ui->setupUi(this);
}


moveDialog::~moveDialog()
{
    delete obj;
    delete ui;
}

void moveDialog::setObject(Object *object)
{
    obj = object;
    culcCenterObj();
    valX=0, valY=0, valZ=0;
    rX=0,rY=0,rZ=0;
    sX=1,sY=0,sZ=0;
}

void moveDialog::culcCenterObj()
{
    double sumX=0,sumY=0,sumZ=0;
    for(int i=0;i<obj->getVertices().size();i++)
    {
        sumX += obj->getVertices()[i].x;
        sumY += obj->getVertices()[i].y;
        sumZ += obj->getVertices()[i].z;
    }
    center.x = sumX/obj->countVertex;
    center.y = sumY/obj->countVertex;
    center.z = sumZ/obj->countVertex;
    ui->doubleSpinBox->setValue(center.x);
    ui->doubleSpinBox_2->setValue(center.y);
    ui->doubleSpinBox_3->setValue(center.z);
}

void moveDialog::rotZ(int angle)
{
    float x,y,z;
    for(int i=0; i<obj->getVertices().size(); i++)
    {
        x = obj->getVertices()[i].x;
        y = obj->getVertices()[i].y;
        z = obj->getVertices()[i].z;
        obj->loadMovedPoint(i,vec3(
                               ((x - center.x) * cos(angle*M_PI/180)) - ((y - center.y) * sin(angle*M_PI/180)) + center.x,
                                ((y - center.y) * cos(angle*M_PI/180)) + ((x - center.x) * sin(angle*M_PI/180))  + center.y ,
                                z
                                ));
    }
    culcCenterObj();
}
void moveDialog::rotY(int angle)
{
    double x,y,z;
    for(int i=0; i<obj->getVertices().size(); i++)
    {
        x = obj->getVertices()[i].x;
        y = obj->getVertices()[i].y;
        z = obj->getVertices()[i].z;
        obj->loadMovedPoint(i,vec3(
                                  (x - center.x) * cos(angle * M_PI/180) + (z - center.z) * sin(angle*M_PI/180) + center.x,
                                   y,
                                   -(x - center.x) * sin(angle * M_PI/180) + (z - center.z) * cos(angle*M_PI/180) + center.z
                                ));
    }
    culcCenterObj();
}
void moveDialog::rotX(int angle)
{
    double x,y,z;
    for(auto i=0; i<obj->getVertices().size(); i++)
    {
        x = obj->getVertices()[i].x;
        y = obj->getVertices()[i].y;
        z = obj->getVertices()[i].z;
        obj->loadMovedPoint(i,vec3(x,
                                   (y - center.y) * cos(angle*M_PI/180) - (z - center.z) * sin(angle*M_PI/180) + center.y,
                                   (y - center.y) * sin(angle*M_PI/180) + (z - center.z) * cos(angle*M_PI/180) + center.z
                                   ));
    }
    culcCenterObj();
}

void moveDialog::moveX()
{
    valX = center.x;
    diff = ui->doubleSpinBox->value() - valX;
    if(diff!=0)
    {
        for(auto i=0; i<obj->getVertices().size(); i++)
        {
            obj->loadMovedPoint(i,vec3(obj->getVertices()[i].x + diff,
                                     obj->getVertices()[i].y,
                                     obj->getVertices()[i].z));
        }
    }
    center.x+=diff;
}

void moveDialog::moveY()
{
    valY = center.y;
    diff = ui->doubleSpinBox_2->value() - valY;
    if(diff!=0)
    {
        for(int i=0; i<obj->getVertices().size(); i++)
        {
            obj->loadMovedPoint(i,vec3(obj->getVertices()[i].x ,
                                     obj->getVertices()[i].y + diff,
                                     obj->getVertices()[i].z));
        }
    }
    center.y+=diff;
}

void moveDialog::moveZ()
{
    valZ = center.z;
    diff = ui->doubleSpinBox_3->value() - valZ;
    if(diff!=0)
    {
        for(int i=0; i<obj->getVertices().size(); i++)
        {
            obj->loadMovedPoint(i,vec3(obj->getVertices()[i].x ,
                                     obj->getVertices()[i].y,
                                     obj->getVertices()[i].z + diff));
        }
    }
    center.z+=diff;
}

void moveDialog::on_doubleSpinBox_valueChanged(double arg1)
{
    moveX();
    emit moved();
}

void moveDialog::on_doubleSpinBox_2_valueChanged(double arg1)
{
    moveY();
    emit moved();
}

void moveDialog::on_doubleSpinBox_3_valueChanged(double arg1)
{
    moveZ();
    emit moved();
}

void moveDialog::on_horizontalSlider_sliderMoved(int position)
{
    ui->spinBox->setValue(position);
    rotX(position - rX);
    emit moved();
    culcCenterObj();
    rX = position;
}

void moveDialog::on_horizontalSlider_2_sliderMoved(int position)
{
    ui->spinBox_2->setValue(position);
    rotY(position - rY);
    emit moved();
    culcCenterObj();
    rY = position;
}

void moveDialog::on_horizontalSlider_3_sliderMoved(int position)
{
    ui->spinBox_3->setValue(position);
    rotZ(position - rZ);
    emit moved();
    culcCenterObj();
    rZ = position;
}

void moveDialog::on_horizontalSlider_7_sliderMoved(int position)
{
    ui->label_10->setText(QString::number((double)position/250));
    sX = (double)position/150 - sX + 1;
    for(int i=0; i<obj->getVertices().size(); i++)
    {
        obj->loadMovedPoint(i,vec3(
                                obj->getVertices()[i].x * sX ,
                                obj->getVertices()[i].y ,
                                obj->getVertices()[i].z
                                ));
    }
    sX = (double)position/150;
    emit moved();
    culcCenterObj();
}

void moveDialog::on_horizontalSlider_6_sliderMoved(int position)
{
    ui->label_14->setText(QString::number((double)position/250));
    sY = (double)position/200 - sY + 1;
    for(int i=0; i<obj->getVertices().size(); i++)
    {
        obj->loadMovedPoint(i,vec3(
                                obj->getVertices()[i].x ,
                                obj->getVertices()[i].y * sY,
                                obj->getVertices()[i].z
                                ));
    }
     sY = (double)position/200;
     emit moved();
     culcCenterObj();
}

void moveDialog::on_horizontalSlider_5_sliderMoved(int position)
{
    ui->label_15->setText(QString::number((double)position/250));
    sZ = (double)position/200 - sZ + 1;
    for(int i=0; i<obj->getVertices().size(); i++)
    {
        obj->loadMovedPoint(i,vec3(
                                obj->getVertices()[i].x ,
                                obj->getVertices()[i].y ,
                                obj->getVertices()[i].z * sZ
                                ));
    }
     sZ = (double)position/200;
     emit moved();
     culcCenterObj();
}

//rotationX
void moveDialog::on_spinBox_valueChanged(int arg1)
{
    rotX(arg1 - rX);
    emit moved();
    culcCenterObj();
    rX = arg1;
}
//rotationY
void moveDialog::on_spinBox_2_valueChanged(int arg1)
{
    rotY(arg1 - rY);
    emit moved();
    culcCenterObj();
    rY = arg1;
}
//rotationZ
void moveDialog::on_spinBox_3_valueChanged(int arg1)
{
    rotZ(arg1 - rZ);
    emit moved();
    culcCenterObj();
    rZ = arg1;
}

