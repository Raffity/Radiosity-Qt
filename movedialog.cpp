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
    delete ui;
}

double oldRotX=0, oldRotY=0, oldRotZ=0, oldScale=0;

void moveDialog::setObject(Object *object)
{
    obj = object;
    double sumX=0,sumY=0,sumZ=0;
    for(int i=0;i<obj->getVertices().size();i++)
    {
        sumX =+ obj->getVertices()[i].x;
        sumY =+ obj->getVertices()[i].y;
        sumZ =+ obj->getVertices()[i].z;
    }
    centerX = sumX/obj->countVertex;
    centerY = sumY/obj->countVertex;
    centerZ = sumZ/obj->countVertex;
    ui->doubleSpinBox->setValue(centerX);
    ui->doubleSpinBox_2->setValue(centerY);
    ui->doubleSpinBox_3->setValue(centerZ);
}


void moveDialog::rotX(double angle)
{
    double x,y,z;
    for(int i=0; i<obj->getVertices().size(); i++)
    {
        x = obj->getVertices()[i].x;
        y = obj->getVertices()[i].y;
        z = obj->getVertices()[i].z;
        obj->loadMovedPoint(i,vec3(
//                                x * cos(angle) + y *sin(angle),
//                                x * sin(angle) + y *cos(angle),
//                                z
                                (x - centerX) *cos(angle) - (y - centerY) *sin(angle) + centerX,
                                (y - centerY) *cos(angle) + (x - centerX) *sin(angle)  + centerY ,
                                z
                                ));
    }
}
void moveDialog::rotY(double angle)
{
    double x,y,z;
    for(int i=0; i<obj->getVertices().size(); i++)
    {
        x = obj->getVertices()[i].x;
        y = obj->getVertices()[i].y;
        z = obj->getVertices()[i].z;
        obj->loadMovedPoint(i,vec3(
                                  (x - centerX) *cos(angle) + (z - centerZ) *sin(angle) + centerX,
                                   y,
                                   -(x - centerX) *sin(angle) + (z - centerZ) *cos(angle) + centerZ
                                ));
    }
}
void moveDialog::rotZ(double angle)
{
    double x,y,z;
    for(auto i=0; i<obj->getVertices().size(); i++)
    {
        x = obj->getVertices()[i].x;
        y = obj->getVertices()[i].y;
        z = obj->getVertices()[i].z;
        obj->loadMovedPoint(i,vec3(x,
                                   (y - centerY) * cos(angle) - (z - centerZ) * sin(angle) + centerY,
                                   (y - centerY) * sin(angle) + (z - centerZ) * cos(angle) + centerZ
                                   ));
    }
}

void moveDialog::moveX()
{
    valX = obj->getVertices()[0].x;
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
}

void moveDialog::moveY()
{
    valY = obj->getVertices()[0].y;
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
}

void moveDialog::moveZ()
{
    valZ = obj->getVertices()[0].z;
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


void moveDialog::on_buttonBox_accepted()
{
    emit moved();
}

void moveDialog::on_horizontalSlider_sliderMoved(int position)
{
    ui->spinBox->setValue(position);
}

void moveDialog::on_horizontalSlider_2_sliderMoved(int position)
{
    ui->spinBox_2->setValue(position);
}

void moveDialog::on_horizontalSlider_3_sliderMoved(int position)
{
    ui->spinBox_3->setValue(position);
}

void moveDialog::on_horizontalSlider_sliderReleased()
{
    rotX(ui->horizontalSlider->value());
    emit moved();
}

void moveDialog::on_horizontalSlider_2_sliderReleased()
{
    rotY(ui->horizontalSlider_2->value());
    emit moved();
}

void moveDialog::on_horizontalSlider_3_sliderReleased()
{
    rotZ(ui->horizontalSlider_3->value());
    emit moved();
}

void moveDialog::on_horizontalSlider_4_sliderReleased()
{
    for(int i=0; i<obj->getVertices().size(); i++)
    {
        obj->loadMovedPoint(i,vec3(
                                obj->getVertices()[i].x * ui->horizontalSlider_4->value() / 100,
                                obj->getVertices()[i].y * ui->horizontalSlider_4->value() / 100,
                                obj->getVertices()[i].z * ui->horizontalSlider_4->value() / 100
                                ));
    }
     emit moved();
}

void moveDialog::on_horizontalSlider_4_sliderMoved(int position)
{
    double pos = double(position);
    ui->label_10->setText(QString::number(pos/100));

}

void moveDialog::on_spinBox_valueChanged(int arg1)
{

}

void moveDialog::on_spinBox_2_valueChanged(int arg1)
{

}

void moveDialog::on_spinBox_3_valueChanged(int arg1)
{

}
