#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

MainWindow *setup(){
    static auto w = new MainWindow(nullptr);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    return w;
}

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    setup()->show();
    return a.exec();
}
