#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <application.h>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    void initializeGL();
    void paintGL();
    void setObect(Application *app);
    void resizeGL(int w,int h);

private:

    void mousePressEvent(QMouseEvent *pe);
    void mouseMoveEvent(QMouseEvent *pe);
    void wheelEvent(QWheelEvent* pe);
    void keyPressEvent(QKeyEvent* pe);
    Application *application;
    QPoint ptrMousePosition;
    GLfloat xRot;//поворот камеры относительно X
    GLfloat yRot;//поворот камеры относительно Y
    GLfloat zRot;//поворот камеры относительно Z
    GLfloat zTra;// перемещение камеры по оси Z( происходит перд поворотом)
    GLfloat nSca;// чувствительность мыши
    void scale_plus();
    void scale_minus();
    void rotate_up();
    void rotate_down();
    void rotate_left();
    void rotate_right();
    void translate_down();
    void translate_up();
    void defaultScene();

};

#endif // GLWIDGET_H
