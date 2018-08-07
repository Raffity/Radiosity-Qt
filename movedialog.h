#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include <QDialog>
#include <object.h>
#include <vec3.h>
#include <vector>
#include <object.h>
#include <vec3.h>

using namespace std;

namespace Ui {
class moveDialog;
}

class moveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit moveDialog(QWidget *parent=0);
    ~moveDialog();
     void setObject(Object *object);

signals:
    void moved();

private slots:
    void on_doubleSpinBox_valueChanged(double arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

    void on_horizontalSlider_7_sliderMoved(int position);

    void on_horizontalSlider_6_sliderMoved(int position);

    void on_horizontalSlider_5_sliderMoved(int position);


private:
    Ui::moveDialog *ui;
    Object *obj;
    float diff=0;
    void moveX();
    void moveY();
    void moveZ();
    void rotX(int angel);
    void rotY(int angel);
    void rotZ(int angel);
    void scaleX(int n);
    void scaleY(int n);
    void scaleZ(int n);
    void culcCenterObj();
    float valX=0, valY=0, valZ=0;
    int rX=0,rY=0,rZ=0;
    double sX=1,sY=0,sZ=0;
    vec3 center = vec3(0,0,0);
};

#endif // MOVEDIALOG_H
