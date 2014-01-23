#include "mainwindow.hpp"
#include <dispatcher.hpp>
#include <processor.hpp>
#include <QApplication>

#include <QThreadPool>

int main(int argc, char *argv[])
{
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

}
