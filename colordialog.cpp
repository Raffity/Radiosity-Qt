#include "colordialog.h"
#include "ui_colordialog.h"
#include <vec3.h>
#include <QColorDialog>
#include <QColor>
#include <QPalette>

ColorDialog::ColorDialog(QWidget *parent, QColor c1, QColor c2,int emissionStrength):
    QDialog(parent),
    ui(new Ui::ColorDialog)
{
    ui->setupUi(this);
    color=c1;
    emissionColor=c2;
    if(emissionColor!= Qt::black)
    {
        ui->checkBox->setChecked(true);
        on_checkBox_clicked();
        ui->spinBox->setValue(emissionStrength);
    }
    QPalette pal=palette();
    QPalette pal1=palette();
    pal.setColor(QPalette::Background,color);
    pal1.setColor(QPalette::Background,emissionColor);
    ui->widget->setPalette(pal);
    ui->widget_2->setPalette(pal1);
    ui->widget->setAutoFillBackground(true);
    ui->widget_2->setAutoFillBackground(true);
    ui->widget->show();
    ui->widget_2->show();
}

ColorDialog::~ColorDialog()
{
    delete ui;
}

void ColorDialog::on_pushButton_clicked()
{
    QColor diaColor=QColorDialog::getColor();
    color=diaColor;
    QPalette pal=palette();
    pal.setColor(QPalette::Background,color);
    ui->widget->setPalette(pal);
}

void ColorDialog::on_pushButton_2_clicked()
{
   QColor diaColor=QColorDialog::getColor();
    emissionColor=diaColor;
    QPalette pal=palette();
    pal.setColor(QPalette::Background,emissionColor);
    ui->widget_2->setPalette(pal);
}

void ColorDialog::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked())
    {
        ui->pushButton_2->setEnabled(true);
        ui->spinBox->setEnabled(true);
    }
    else{
        ui->pushButton_2->setEnabled(false);
        emissionColor=Qt::black;
        QPalette pal=palette();
        pal.setColor(QPalette::Background,Qt::black);
        ui->widget_2->setPalette(pal);
        ui->spinBox->setEnabled(false);
    }
}

void ColorDialog::on_buttonBox_accepted()
{
    emit giveColors(color,emissionColor,ui->spinBox->value());
}
