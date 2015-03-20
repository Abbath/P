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
    QSqlError addSensor(QString name, const ModelingData& data);
    QSqlError addConf(Config config);
    QSqlError addData(const QVector<double> &pix, const QVector<double> &press);
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
    explicit DB(QObject *parent = 0);
    DB(const DB& src);
    DB& operator=(const DB& rhs);
    QPair<QVector<int>, QVector<int> > getCDIDs();
};

#endif // DB_HPP
