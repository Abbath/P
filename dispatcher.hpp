#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <imagearea.hpp>
#include <imageprocessor.hpp>
#include <QObject>

class Dispatcher : public QObject
{
    Q_OBJECT
    ImageArea * ia;
    ImageProcessor * ip;
public:
    explicit Dispatcher(QObject *parent = 0);
    void openImage();
    void openVideo();
    void saveImage();
    void saveConf();
    void loadConf();
    void calibrate();
    void saveResults();
    void loadData();
    void saveData();
    
signals:
    
public slots:
    
};

#endif // DISPATCHER_HPP
