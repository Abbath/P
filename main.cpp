#include "mainwindow.hpp"
#include <processor.hpp>
#include <QApplication>

#include <QThreadPool>

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    
    MainWindow w;
    Processor p;
    w.setProcessor(&p);
    qRegisterMetaType<Display>("Display");
    QObject::connect(&p, SIGNAL(Update(Display)), &w, SLOT(Update(Display)),Qt::ConnectionType::QueuedConnection);
    QObject::connect(&p, SIGNAL(somethingWentWrong(QString,QString)),&w, SLOT(Error(QString,QString)),Qt::ConnectionType::QueuedConnection);
    p.loadConf("default.conf");  
    w.show();
    return a.exec();
}
