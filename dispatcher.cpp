#include "dispatcher.hpp"

#include <cassert>
#include "processor.hpp"
#include "mainwindow.hpp"

Dispatcher::Dispatcher(QObject *parent, MainWindow *w, Processor *p) :
  QObject(parent),
  w(w),
  p(p)
{
  assert(w != nullptr);
  assert(p != nullptr);
  connect(this->w, SIGNAL(openImage(QStringList)), this->p, SLOT(openImage(QStringList)), Qt::ConnectionType::QueuedConnection);
  connect(this->p, SIGNAL(Update(Display)), this->w, SLOT(Update(Display)),Qt::ConnectionType::QueuedConnection);
}
