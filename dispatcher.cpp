#include "dispatcher.hpp"

#include <cassert>
#include "processor.hpp"
#include "mainwindow.hpp"

Dispatcher::Dispatcher(QObject *parent, MainWindow *w, Processor *p) :
  QObject(parent),
  w(w),
  p(p)
{
<<<<<<< HEAD
    qRegisterMetaType<QVector<std::string>>("QVector<std::string>");
    qRegisterMetaType<Display>("Display");
    connect(this->w, SIGNAL(openImage(QVector<std::string>)), this->p, SLOT(openImage(QVector<std::string>)),Qt::ConnectionType::QueuedConnection);
    connect(this->p, SIGNAL(Update(Display)), this->w, SLOT(Update(Display)),Qt::ConnectionType::QueuedConnection);
    connect(this->p, SIGNAL(somethingWentWrong(QString,QString)),this->w, SLOT(Error(QString,QString)));
    connect(this->w, SIGNAL(align()),this->p, SLOT(align()));
    connect(this->w, SIGNAL(autorun()),this->p, SLOT(autorun()));
    connect(this->w, SIGNAL(loadConf(QString)), this->p, SLOT(loadConf(QString)));
    connect(this->w, SIGNAL(saveConf(QString,bool)), this->p, SLOT(saveConf(QString,bool)));
    connect(this->w, SIGNAL(loadData(QString)), this->p, SLOT(loadData(QString)));
    connect(this->w, SIGNAL(saveData(QString)), this->p, SLOT(saveData(QString)));
=======
  assert(w != nullptr);
  assert(p != nullptr);
  connect(this->w, SIGNAL(openImage(QStringList)), this->p, SLOT(openImage(QStringList)), Qt::ConnectionType::QueuedConnection);
  connect(this->p, SIGNAL(Update(Display)), this->w, SLOT(Update(Display)),Qt::ConnectionType::QueuedConnection);
>>>>>>> b1ab59f05675701a3fd54cf8420a16f90efe4b53
}
