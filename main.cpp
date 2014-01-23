#include "mainwindow.hpp"
#include <dispatcher.hpp>
#include <processor.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    MainWindow w;
    Processor p;
    p.moveToThread(new QThread);
    Dispatcher d(0, &w, &p);
    d.moveToThread(new QThread);
    w.show();
    return a.exec();
}
