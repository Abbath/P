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
  QThread t;
  p.moveToThread(&t);
  t.start();  
  //qRegisterMetaType<QVector<std::string>>("QVector<std::string>");
  qRegisterMetaType<Display>("Display");
  QObject::connect(&w, SIGNAL(openImage(QStringList)), &p, SLOT(openImage(QStringList)), Qt::ConnectionType::QueuedConnection);
  QObject::connect(&p, SIGNAL(Update(Display)), &w, SLOT(Update(Display)),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&w, SIGNAL(openImage(QStringList)), &p, SLOT(openImage(QStringList)),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&p, SIGNAL(Update(Display)), &w, SLOT(Update(Display)),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&p, SIGNAL(somethingWentWrong(QString,QString)),&w, SLOT(Error(QString,QString)),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&w, SIGNAL(align()),&p, SLOT(align()),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&w, SIGNAL(autorun()),&p, SLOT(autorun()),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&w, SIGNAL(reset()),&p, SLOT(reset()),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&w, SIGNAL(run()),&p, SLOT(run()),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&w, SIGNAL(loadConf(QString)), &p, SLOT(loadConf(QString)),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&w, SIGNAL(saveConf(QString,bool)), &p, SLOT(saveConf(QString,bool)),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&w, SIGNAL(loadData(QString)), &p, SLOT(loadData(QString)),Qt::ConnectionType::QueuedConnection);
  QObject::connect(&w, SIGNAL(saveData(QString)), &p, SLOT(saveData(QString)),Qt::ConnectionType::QueuedConnection);
//  Dispatcher d(0, &w, &p);
//  d.moveToThread(new QThread);
  w.show();
  return a.exec();


}
