#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <QObject>

QT_BEGIN_NAMESPACE
class MainWindow;
class Processor;
QT_END_NAMESPACE

class Dispatcher : public QObject
{
  Q_OBJECT
public:
  explicit Dispatcher(QObject *parent = 0, MainWindow* w = nullptr, Processor * p = nullptr);

private:
  MainWindow * w;
  Processor * p;
};

#endif // DISPATCHER_HPP
