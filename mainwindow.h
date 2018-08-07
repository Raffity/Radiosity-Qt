#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <application.h>
#include <colordialog.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void disp();
    void colorSelected(QColor c1,QColor c2,int strength);
    void on_button_1_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_action_changed();
    void on_action_triggered();
    void on_action_2_triggered();
    void on_action_3_triggered();
    void on_action_4_triggered();
    void on_pushButton_10_clicked();
    void resizeEvent(QResizeEvent* event);

    void on_listWidget_clicked(const QModelIndex &index);

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_comboBox_3_currentIndexChanged(const QString &arg1);

    void on_button_2_clicked();

    void on_MainWindow_iconSizeChanged(const QSize &iconSize);

private:
    Ui::MainWindow *ui;
    Application *app = NULL;
    bool check=false;
};

#endif // MAINWINDOW_H
