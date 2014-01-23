#include "mainwindow.hpp"
#include <dispatcher.hpp>
#include <processor.hpp>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    MainWindow w;
    Processor p;
    QThread tfd;
    QThread tfp;
    p.moveToThread(&tfp);
    Dispatcher d(0, &w, &p);
    d.moveToThread(&tfd);
    tfp.start();
    tfd.start();
    w.show();
    return a.exec();
}
