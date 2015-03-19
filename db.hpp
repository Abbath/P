#ifndef DB_HPP
#define DB_HPP

#include <QObject>
#include <QtSql>
#include "helpers.hpp"

class DB : public QObject
{
    Q_OBJECT
public:
    explicit DB(QObject *parent = 0);
    ~DB();
    QSqlError init();
    QSqlError addSensor(QString name, const ModelingData& data, int confId, int dataId);
    QSqlError addConf(Config config);
    QSqlError addData(const QVector<double> &pix, const QVector<double> &press);
signals:
    
public slots:
    
private:
    QSqlDatabase db;
};

#endif // DB_HPP
