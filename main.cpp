#include "mainwindow.hpp"
#include <processor.hpp>
#include <QApplication>
#include <QtGui>
#include <QThreadPool>
#include <QSplashScreen>
#include <dialog.hpp>
int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    MainWindow w;
    Processor p;
    w.setProcessor(&p);
    qRegisterMetaType<Display>("Display");
    qRegisterMetaType<DataType>("DataType");
    qRegisterMetaType<QVector<double>>("QVector<double>");
    QObject::connect(&p, SIGNAL(Update(Display)), &w, SLOT(Update(Display)),Qt::ConnectionType::QueuedConnection);
    QObject::connect(&p, SIGNAL(somethingWentWrong(QString,QString)),&w, SLOT(Error(QString,QString)),Qt::ConnectionType::QueuedConnection);
    QObject::connect(&p,SIGNAL(plot(DataType,QVector<double>)),&w,SLOT(plot(DataType,QVector<double>)),Qt::QueuedConnection);
    QObject::connect(&p,SIGNAL(plot(QVector<double>,QVector<double>)),&w,SLOT(plot(QVector<double>,QVector<double>)),Qt::QueuedConnection);
    
    p.loadConf("default.conf");  
    w.show();
    Dialog d;
    d.setModal(true);
    d.show();
    QObject::connect(&d,SIGNAL(rejected()), &w, SLOT(close()));
    return a.exec();
}
