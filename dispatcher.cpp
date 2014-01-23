#include "dispatcher.hpp"

Dispatcher::Dispatcher(QObject *parent, MainWindow *w, Processor *p) :
    QObject(parent),
    w(w),
    p(p)
{
    qRegisterMetaType<QVector<std::string>>("QVector<std::string>");
    qRegisterMetaType<Display>("Display");
    connect(this->w, SIGNAL(openImage(QVector<std::string>)), this->p, SLOT(openImage(QVector<std::string>)),Qt::ConnectionType::QueuedConnection);
    connect(this->p, SIGNAL(Update(Display)), this->w, SLOT(Update(Display)),Qt::ConnectionType::QueuedConnection);
}
