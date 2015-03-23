#ifndef SENSOR_HPP
#define SENSOR_HPP

#include <QObject>
#include "modelingdata.hpp"
#include "config.hpp"

class Sensor : public QObject{
    Q_OBJECT
    ModelingData data;
    Config conf;
    QVector<double> pix;
    QVector<double> pres;
public:
    ModelingData getModelingData() const
    {
        return data;
    }
    
    void setModelingData(const ModelingData &value)
    {
        data = value;
    }
    
    Config getConf() const
    {
        return conf;
    }
    
    void setConf(const Config &value)
    {
        conf = value;
    }

    QPair<QVector<double>, QVector<double>> getMeasurementData() const{
        return QPair<QVector<double>, QVector<double>>(pix, pres);
    }
    
    void setMeasurementData(QVector<double> p, QVector<double> pr){
        pix = p;
        pres = pr;
    }
    
    void setMeasurementData(QPair<QVector<double>, QVector<double>> data){
        pix = data.first;
        pres = data.second;
    }
    ~Sensor();
    Sensor();
};

#endif // SENSOR_HPP
