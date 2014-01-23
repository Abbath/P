#include "mainwindow.hpp"
#include <dispatcher.hpp>
#include <processor.hpp>
#include <QApplication>

#include <QThreadPool>

int main(int argc, char *argv[])
{
<<<<<<< HEAD
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
=======
  QApplication a(argc, argv);

  MainWindow w;
  Processor p;
  p.moveToThread(new QThread);

  QObject::connect(&w, SIGNAL(openImage(QStringList)), &p, SLOT(openImage(QStringList)), Qt::ConnectionType::QueuedConnection);
  QObject::connect(&p, SIGNAL(Update(Display)), &w, SLOT(Update(Display)),Qt::ConnectionType::QueuedConnection);

//  Dispatcher d(0, &w, &p);
//  d.moveToThread(new QThread);
  w.show();
  return a.exec();

>>>>>>> b1ab59f05675701a3fd54cf8420a16f90efe4b53
}
