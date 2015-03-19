#ifndef DB_HPP
#define DB_HPP

#include <QObject>
#include <QtSql>
#include <memory>
#include <mutex>
#include "helpers.hpp"

class DB : public QObject
{
    Q_OBJECT
public:
    virtual ~DB();
    QSqlError init();
    QSqlError addSensor(QString name, const ModelingData& data, int confId, int dataId);
    QSqlError addConf(Config config);
    QSqlError addData(const QVector<double> &pix, const QVector<double> &press);
    QStringList getSensorNames();
    static DB& getInstance();
signals:
    
public slots:
    
private:
    QSqlDatabase db;
    static std::unique_ptr<DB> instance;
    static std::once_flag onceFlag;
    explicit DB(QObject *parent = 0);
    DB(const DB& src);
    DB& operator=(const DB& rhs);
};

#endif // DB_HPP
