#ifndef DB_HPP
#define DB_HPP

#include <QObject>
#include <QtSql>
#include <memory>
#include <mutex>
#include "config.hpp"
#include "modelingdata.hpp"
#include "sensor.hpp"

class DB
{
public:
    virtual ~DB();
    QSqlError init();
    QSqlError addFullSensor(QString name, const Sensor& sensor);
    QSqlError addSensor(QString name, const ModelingData& data);
    QSqlError addConf(Config config);
    QSqlError addData(const QVector<double> &pix, const QVector<double> &press);
    QSqlError removeSensor(QString name);
    ModelingData getSensor(QString name, int &cid, int &did);
    Config getConfig(int id);
    QPair<QVector<double>, QVector<double>> getData(int id);
    QStringList getSensorNames();
    static DB& getInstance();
    int getConfId() const;
    void setConfId(int value);
    
    int getDataId() const;
    void setDataId(int value);
    
signals:
    
public slots:
    
private:
    QSqlDatabase db;
    int confId;
    int dataId;
    static std::unique_ptr<DB> instance;
    static std::once_flag onceFlag;
    explicit DB();
    DB(const DB& src);
    DB& operator=(const DB& rhs);
    QPair<QVector<int>, QVector<int> > getCDIDs();
};

#endif // DB_HPP
