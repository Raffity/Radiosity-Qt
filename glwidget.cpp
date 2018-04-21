#include "glwidget.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <GL/glu.h>

bool created = false;

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
    this->setFocus();
    defaultScene();
}

void GLWidget::setObect(Application *app)
{
    application = app;
    created =true;
}

void GLWidget::mousePressEvent(QMouseEvent *pe)
{
     ptrMousePosition = pe->pos();
     this->setFocus();
}

void GLWidget::mouseMoveEvent(QMouseEvent *pe)
{
    if(created){
        xRot  = 180/nSca*(GLfloat)(pe->y()-ptrMousePosition.y())/height();
        zRot  = 180/nSca*(GLfloat)(pe->x()-ptrMousePosition.x())/width();
        application->camera.yaw -= zRot ;
        application->camera.pitch -= xRot ;
        application->camera.applyTransformation();
        if(application->dispFunc) {application->reshapeFunc();}
        else {application->preview();}
        ptrMousePosition = pe->pos();
        updateGL();
    }
}

void GLWidget::initializeGL()
{

}

void GLWidget::paintGL()
{
}

void GLWidget::resizeGL(int w, int h)
{

}

void GLWidget::wheelEvent(QWheelEvent* pe)
{
   if ((pe->delta())>0) scale_plus(); else if ((pe->delta())<0) scale_minus();
   updateGL();
}

void GLWidget::keyPressEvent(QKeyEvent* pe)
{
   switch (pe->key())
   {
      case Qt::Key_A:
         application->camera.position = application->camera.position + vec3(0,0,zTra);
         application->camera.applyTransformation();
         if(application->dispFunc) {application->reshapeFunc();}
         else {application->preview();}
         updateGL();
      break;

      case Qt::Key_D:
         application->camera.position = application->camera.position - vec3(0,0,zTra);
         application->camera.applyTransformation();
         if(application->dispFunc) {application->reshapeFunc();}
         else {application->preview();}
         updateGL();
      break;

   case Qt::Key_W:
      application->camera.position = application->camera.position + vec3(0,zTra,0);
      application->camera.applyTransformation();
      if(application->dispFunc) {application->reshapeFunc();}
      else {application->preview();}
      updateGL();
   break;

   case Qt::Key_S:
      application->camera.position = application->camera.position - vec3(0,zTra,0);
      application->camera.applyTransformation();
      if(application->dispFunc) {application->reshapeFunc();}
      else {application->preview();}
      updateGL();
   break;
   case Qt::Key_E:
      application->camera.position = application->camera.position + vec3(zTra,0,0);
      application->camera.applyTransformation();
      if(application->dispFunc) {application->reshapeFunc();}
      else {application->preview();}
      updateGL();
   break;

   case Qt::Key_Q:
      application->camera.position = application->camera.position - vec3(zTra,0,0);
      application->camera.applyTransformation();
      if(application->dispFunc) {application->reshapeFunc();}
      else {application->preview();}
      updateGL();
   break;
   }
}

void GLWidget::scale_plus()
{
   nSca = nSca*1.1;
}

void GLWidget::scale_minus()
{
   nSca = nSca/1.1;
}

void GLWidget::rotate_up()
{
   xRot  = 1.0;
}

void GLWidget::rotate_down()
{
   xRot -= 1.0;
}

void GLWidget::rotate_left()
{
   zRot  = 1.0;
}

void GLWidget::rotate_right()
{
   zRot -= 1.0;
}

void GLWidget::translate_down()
{
   zTra -= 0.05;
}

void GLWidget::translate_up()
{
   zTra  = 0.05;
}

void GLWidget::defaultScene()
{
   xRot=0; yRot=0; zRot=0; zTra=1; nSca=1;
}


