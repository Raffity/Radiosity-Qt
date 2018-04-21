#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#include <QDialog>
#include <vec3.h>
#include <QColorDialog>
#include <QColor>

namespace Ui {
class ColorDialog;
}

class ColorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorDialog(QWidget *parent, QColor c1,  QColor c2);
    ~ColorDialog();

signals:
    void giveColors(QColor c1,QColor c2,int strength);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_checkBox_clicked();
    void on_buttonBox_accepted();

private:
    Ui::ColorDialog *ui;
    QColor color;
    QColor emissionColor;
    int strength;
};

#endif // COLORDIALOG_H
