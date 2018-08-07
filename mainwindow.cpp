#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "application.h"
#include <QtGui>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <colordialog.h>
#include <string.h>
#include <movedialog.h>
#include <QStyleFactory>
#include <GL/gl.h>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//событие изменения размера окна
 void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if(app != NULL)
    {
        app->height = ui->OpenGL->height();
        app->width = ui->OpenGL->width();
        disp();
    }
    else
    {
        glViewport(0, 0, 400,400);
        glOrtho(-24, 24, -24, 24, 0.01, 10000);
        glMatrixMode(GL_MODELVIEW);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        ui->OpenGL->update();
    }
}

//Кнопка загрузить сцену
void MainWindow::on_button_1_clicked()
{
    if(app != NULL)
    {
        delete app;
    }
    ui->listWidget->clear();
    app = new Application();
    app->height = ui->OpenGL->height();
    app->width = ui->OpenGL->width();
    QString filename=QFileDialog::getOpenFileName(this,
                                                  "Открытие файла",
                                                  "",
                                                  "obj(*.obj)");
    QFileInfo pathName(filename);
    ui->label_4->setText(pathName.baseName());
    if (!app->init(filename))
    {
        QMessageBox::warning(this,
                                 "Ошибка",
                                 "Пожалуйста, выберите корректный файл,объекты должны быть триангулированы и файл не должен содержать нормали");
    }
    else{
        ui->spinBox->setEnabled(true);
        ui->comboBox->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_12->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->listWidget->addItems(app->getObjects());
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QString::fromUtf8("MTL"),
        QString::fromUtf8("Загрузить MTL файл?"),
        QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QString filename=QFileDialog::getOpenFileName(this,
                                                          "Открытие файла",
                                                          "",
                                                          "mtl(*.mtl)");
            if(app->loadFromMTLFile(filename))
            {
                QMessageBox::about(this,"MTL","MTL файл успешно загружен");
                app->searchMTLtoOBJ();
            }
        }
         app->previewCreator();
         app->preview();
    }
    ui->OpenGL->setObect(app);
    ui->label_10->setText(QString::number(app->totalFaces));
    ui->label_12->setText(QString::number(app->totalVertex));
}



void MainWindow::disp()
{
    if(check && !app->dispFunc ){
        app->style = GL_FILL;
        app->previewCreator();
        app->preview();
    }
    else if(!check && !app->dispFunc){
        app->style = GL_LINE;
        app->camera.applyTransformation();
        app->previewCreator();
        app->preview();
    }
    else if(app->dispFunc)
    {
        app->reshapeFunc();
    }
    ui->OpenGL->update();
}

void MainWindow::on_pushButton_clicked()
{
    app->style = GL_FILL;
    if(!app->dispFunc )
    {
        app->displayFunc();
    }
    app->idleFunc(ui->spinBox->text().toInt(),
                 ui->comboBox->itemText(ui->comboBox->currentIndex()).toInt());
    app->reshapeFunc();
    ui->pushButton_2->setEnabled(false);
    ui->OpenGL->update();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!check){
        app->style = GL_FILL;
        app->preview();
        ui->OpenGL->update();
        check=true;
    }
    else{
        app->style = GL_LINE;
        app->preview();
        ui->OpenGL->update();
        check=false;
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QString file = QFileDialog::getSaveFileName(
                this,
                "Сохранение файла",
                "C://",
                "PNG (*.png);; BMP (*.bmp);; TIFF (*.tiff);; JPEG (*.jpeg)");
    ui->OpenGL->setVisible(true);
    ui->OpenGL->grabFrameBuffer().save(file);
}

void MainWindow::on_action_triggered()
{
    MainWindow::on_button_1_clicked();
}

void MainWindow::on_action_2_triggered()
{
    MainWindow::on_pushButton_3_clicked();
}

void MainWindow::on_action_3_triggered()
{
    MainWindow::close();
}

void MainWindow::on_action_4_triggered()
{
    QMessageBox::information(this,
                             "Info",
                             "Данное приложение визуализирует файлы формата .obj методом излучательности, который реализован в библиотеке Radiosity"
                              );
}


int colorCountt=1;
void MainWindow::colorSelected(QColor c1,QColor c2,int strength)
{
    app->objects[ui->listWidget->currentRow()].setColor(c1);
    app->objects[ui->listWidget->currentRow()].setEmissionColor(c2,strength);
    colorMTL newColor;
    newColor.name = "Color_"+std::to_string(colorCountt);
    newColor.color=vec3(c1.redF(),c1.greenF(),c1.blueF());
    newColor.emissionColor=vec3(c2.redF()*strength,c2.greenF()*strength,c2.blueF()*strength);
    app->colors.push_back(newColor);
    app->objects[ui->listWidget->currentRow()].mtl="Color_"+std::to_string(colorCountt);
    colorCountt++;
}

void MainWindow::on_pushButton_10_clicked()
{
    if(ui->listWidget->currentRow()>=0)
    {
        ColorDialog *colorDialog=new ColorDialog(this,app->objects[ui->listWidget->currentRow()].getColor(),
                app->objects[ui->listWidget->currentRow()].getEmissionColor(),
                app->objects[ui->listWidget->currentRow()].getEmissionStrength());
        connect(colorDialog,SIGNAL(giveColors(QColor,QColor,int)),this,SLOT(colorSelected(QColor,QColor,int)));
        colorDialog -> setModal(true);
        colorDialog -> exec();
        app->objects[ui->listWidget->currentRow()].selected=false;
        disp();
    }
    else{
        QMessageBox::warning(this,
                                 "Error",
                                 "Пожалуйста выберите один из объектов в списке");
    }
}

int old_index=0;
void MainWindow::on_listWidget_clicked(const QModelIndex &index)
{
   if(app->objects.size() > old_index)
   {
       app->objects[old_index].selected = false;
   }
    app->objects[index.row()].selected = true;
    disp();
    ui->OpenGL->update();
    old_index=index.row();
}

void MainWindow::on_pushButton_11_clicked()
{
    app->saveSceneToObj();
    QMessageBox::about(this,"MTL и OBJ","MTL и OBJ файлы успешно сохранены/перезаписаны");
}

void MainWindow::on_pushButton_12_clicked()
{
    if(ui->listWidget->currentRow()>=0)
    {
        moveDialog *md=new moveDialog();
        connect(md,SIGNAL(moved()),this,SLOT(disp()),Qt::DirectConnection);
        md->setObject(app->getObject(ui->listWidget->currentRow()));
        md->show();
        md->exec();
    }
    else{
        QMessageBox::warning(this,
                                 "Error",
                                 "Пожалуйста выберите один из объектов в списке");
    }
}

void MainWindow::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
    QStringList list;
    list.clear();
    ui->comboBox_2->clear();
    if (arg1 == "Плоскость")
    {
        list.append("2");
        list.append("8");
        list.append("32");
        list.append("128");
        list.append("512");
        ui->comboBox_2->addItems(list);
    }
    else if (arg1 == "Куб")
    {
        list.append("12");
        list.append("48");
        list.append("192");
        list.append("768");
        ui->comboBox_2->addItems(list);

    }
    else if (arg1 == "Конус")
    {
        list.append("34");
        list.append("136");
        ui->comboBox_2->addItems(list);
    }
    else if (arg1 == "Сфера")
    {
        list.append("80");
        list.append("256");
        list.append("1024");
        ui->comboBox_2->addItems(list);
    }
    else if (arg1 == "Цилиндр")
    {
        list.append("68");
        list.append("272");
        ui->comboBox_2->addItems(list);
    }

}

void MainWindow::on_button_2_clicked()
{
    if(app != NULL)
    {
        delete app;
    }
    ui->listWidget->clear();
    app = new Application();
    app->height = ui->OpenGL->height();
    app->width = ui->OpenGL->width();
    app->init(NULL);
    QString str = QString("Плоскость_8");
    QString str1 = QString("Куб_48");
    app->addExistingObj(str);
    app->addExistingObj(str1);
    ui->spinBox->setEnabled(true);
    ui->comboBox->setEnabled(true);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_3->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_11->setEnabled(true);
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_6->setEnabled(true);
    ui->comboBox_2->setEnabled(true);
    ui->comboBox_3->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->listWidget->addItems(app->getObjects());
    ui->label_4->setText("radiosity_scene");
    app->previewCreator();
    app->preview();
    ui->OpenGL->update();
    ui->OpenGL->setObect(app);
    ui->label_10->setText(QString::number(app->totalFaces));
    ui->label_12->setText(QString::number(app->totalVertex));
}


void MainWindow::on_pushButton_5_clicked()
{
    if(ui->listWidget->currentRow()>=0)
    {
         app->totalVertex = app->totalVertex - app->objects[ui->listWidget->currentRow()].countVertex;
         app->objects.remove(ui->listWidget->currentRow());
         ui->listWidget->clear();
         ui->listWidget->addItems(app->getObjects());
         ui->listWidget->setCurrentRow(0);
         disp();
         ui->OpenGL->update();
    }
    else
    {
        QMessageBox::warning(this,
                                 "Error",
                                 "Пожалуйста выберите один из объектов в списке");
    }
}


void MainWindow::on_pushButton_4_clicked()
{
    QString str = ui->comboBox_3->currentText() + "_" + ui->comboBox_2->currentText();
    app->addExistingObj(str);
    ui->listWidget->clear();
    ui->listWidget->addItems(app->getObjects());
    app->previewCreator();
    app->preview();
    ui->OpenGL->update();
    ui->label_10->setText(QString::number(app->totalFaces));
    ui->label_12->setText(QString::number(app->totalVertex));
}

int countCopyObj=1;
void MainWindow::on_pushButton_6_clicked()
{
    if(ui->listWidget->currentRow()>=0)
    {
        Object copyObj;
        copyObj = app->objects[ui->listWidget->currentRow()];
        copyObj.setName(copyObj.getName() + "_" + std::to_string(countCopyObj));
        app->objects.push_back(copyObj);
        ui->listWidget->clear();
        ui->listWidget->addItems(app->getObjects());
        ui->listWidget->setCurrentRow(0);
        disp();
        ui->OpenGL->update();
        countCopyObj++;
        app->totalFaces += copyObj.countFaces;
        app->totalVertex += copyObj.countVertex;
        ui->label_10->setText(QString::number(app->totalFaces));
        ui->label_12->setText(QString::number(app->totalVertex));
    }
    else{
        QMessageBox::warning(this,
                                 "Error",
                                 "Пожалуйста выберите один из объектов в списке");
    }
}


void MainWindow::on_pushButton_7_clicked()
{

}

void MainWindow::on_pushButton_8_clicked()
{

}

void MainWindow::on_pushButton_9_clicked()
{
}

void MainWindow::on_action_changed()
{

}

void MainWindow::on_MainWindow_iconSizeChanged(const QSize &iconSize)
{
}
