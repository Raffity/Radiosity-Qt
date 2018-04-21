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


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Кнопка загрузить сцену
void MainWindow::on_button_1_clicked()
{
    ui->listWidget->clear();
    app = new Application();
    QString filename=QFileDialog::getOpenFileName(this,
                                                  "Открытие файла",
                                                  "",
                                                  "obj(*.obj)");
    QFileInfo pathName(filename);
    ui->label_4->setText(pathName.baseName());

    if (!app->init(filename,ui->OpenGL->height(),ui->OpenGL->width()))
    {
        QMessageBox::warning(this,
                                 "Problem",
                                 "Пожалуйста выберите корректный файл");
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
         app->preview();
    }

    ui->OpenGL->setObect(app);
}

void MainWindow::disp()
{
    if(check && !app->dispFunc ){
        app->style = GL_FILL;
        app->preview();
    }
    else if(!check && !app->dispFunc){
        app->style = GL_LINE;
        app->camera.applyTransformation();
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
    { app->displayFunc();}
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

void MainWindow::on_pushButton_4_clicked()
{

}

void MainWindow::on_pushButton_5_clicked()
{

}

void MainWindow::on_pushButton_6_clicked()
{

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
                             "Данное приложение визуализирует файлы формата .obj методом излучательности, который реализован в библиотеке Radiosity, автор которой Sabin Temalsena"
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
                app->objects[ui->listWidget->currentRow()].getEmissionColor());
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
        app->objects[old_index].selected=false;
        app->objects[index.row()].selected=true;
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

        QFile file("dark.stylesheet.qss");
        if(file.open(QFile::ReadOnly | QFile::Text))
           md->setStyleSheet(file.readAll());
        else
            md->setStyle(QStyleFactory::create("Fusion"));

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

