#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include <QDialog>
#include <object.h>
#include <vec3.h>
#include <vector>
#include <object.h>

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
    void on_buttonBox_accepted();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_3_valueChanged(int arg1);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

    void on_horizontalSlider_3_sliderMoved(int position);

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_2_sliderReleased();

    void on_horizontalSlider_3_sliderReleased();

    void on_horizontalSlider_4_sliderReleased();

    void on_horizontalSlider_4_sliderMoved(int position);

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_doubleSpinBox_3_valueChanged(double arg1);

private:
    Ui::moveDialog *ui;
    Object *obj;

    float diff=0;
    void moveX();
    void moveY();
    void moveZ();
    void rotX(double angel);
    void rotY(double angel);
    void rotZ(double angel);
    double valX=0;
    double valY=0;
    double valZ=0;
    double centerX;
    double centerY;
    double centerZ;
};

#endif // MOVEDIALOG_H
