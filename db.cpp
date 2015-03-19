#include "db.hpp"

DB::DB(QObject *parent) : QObject(parent)
{

}

DB::~DB()
{

}

QSqlError DB::init()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./sensors.db");
    
    if(!db.open()){
        return db.lastError();
    }
    
    QStringList tables = db.tables();
    
    if(tables.contains("sensors") && tables.contains("configs") && tables.contains("datum")){
        return QSqlError();
    }
    
    QSqlQuery q;
    
    if(!q.exec(QString("create table sensors(id integer primary key, name varchar, dsize integer, msize integer, mthick integer, sheight integer, pressure real, wl integer, xa real, ya real, cxa real, cya real, rows integer, hs integer, ss integer, ym real, pr real, conf integer, data integer)"))){
        return db.lastError();
    }
    
    if(!q.exec(QString("create table configs(id integer primary key, cr1x integer, cr1y integer, cr2x integer, cr2y integer, sq1x integer, sq1y integer, sq2x integer, sq2y integer, sq3x integer, sq3y integer, sq01x integer, sq01y integer, sq02x integer, sq02y integer, sq03x integer, sq03y integer)"))){
        return db.lastError();
    }
    
    if(!q.exec(QString("create table datum(id integer primary key, data varchar)"))){
        return db.lastError();
    }
    
    if(!q.exec(QString("insert into sensors(id, name, dsize, msize, mthick, sheight, pressure, wl, xa, ya, cxa, cya, rows, hs, ss, ym, pr, conf, data) values(0, \"sensor0\", 15, 5, 20, 20, 40, 535, 0.0, 0.0, 0.0, 30.0, 6, 60, 340, 130.0, 0.27, 0, 0)"))){
            return db.lastError();
    }
    
    return QSqlError();
}

QSqlError DB::addSensor(QString name, const ModelingData &data, int confId, int dataId)
{
    if(!db.isOpen()){
        if(!db.open()){
            return db.lastError();
        }
        db.setDatabaseName("./sensors.db");
    }
    
    
    QSqlQuery q(db);
    QString sensor = "";
   // sensor += QString::number(1);
   // sensor += ", ";
    
    sensor += "\"" + name + "\"";
    sensor += ", ";
    
    sensor += QString::number((int)(data.getDie_size()*1000));
    sensor += ", ";
    
    sensor += QString::number((int)(data.getMem_size()*1000));
    sensor += ", ";
    
    sensor += QString::number((int)(data.getMem_thickness()*1000000));
    sensor += ", ";
    
    sensor += QString::number((int)(data.getSpacer_height()*100));
    sensor += ", ";
    
    sensor += QString::number(data.getPressure()*25/1000);
    sensor += ", ";
    
    sensor += QString::number(data.getWavelength());
    sensor += ", ";
    
    sensor += QString::number(data.getX_angle()*57.3);
    sensor += ", ";
    
    sensor += QString::number(data.getY_angle()*57.3);
    sensor += ", ";
    
    sensor += QString::number(data.getCamx()*57.3);
    sensor += ", ";
    
    sensor += QString::number(data.getCamy()*57.3);
    sensor += ", ";
    
    sensor += QString::number(data.getRows());
    sensor += ", ";
    
    sensor += QString::number(data.getHole_size());
    sensor += ", ";
    
    sensor += QString::number(data.getSpace_size());
    sensor += ", ";
    
    sensor += QString::number(data.getYm()/1e9);
    sensor += ", ";
    
    sensor += QString::number(data.getPr());
    sensor += ", ";
    
    sensor += QString::number(confId);
    sensor += ", ";
    
    sensor += QString::number(dataId);
    
    QString query = "insert into sensors(name, dsize, msize, mthick, sheight, pressure, wl, xa, ya, cxa, cya, rows, hs, ss, ym, pr, conf, data) values(" + sensor + ")";
    
    if(!q.exec(query)){
        return db.lastError();
    }
    
    return QSqlError();
}

QSqlError DB::addConf(Config config)
{
    if(!db.isOpen()){
        if(!db.open()){
            return db.lastError();
        }
    }
    
    db.setDatabaseName("./sensors.db");
    
    QSqlQuery q(db);
    QString conf = "100, ";
    
//    conf += "\"" + name + "\"";
//    conf += ", ";
    
    conf += QString::number(config.crop.left());
    conf += ", ";
    
    
    conf += QString::number(config.crop.top());
    conf += ", ";
    
    conf += QString::number(config.crop.right());
    conf += ", ";
    
    conf += QString::number(config.crop.bottom());
    conf += ", ";
    
    conf += QString::number(config.square[0].x());
    conf += ", ";
    
    conf += QString::number(config.square[0].y());
    conf += ", ";
    
    conf += QString::number(config.square[1].x());
    conf += ", ";
    
    conf += QString::number(config.square[1].y());
    conf += ", ";
    
    conf += QString::number(config.square[2].x());
    conf += ", ";
    
    conf += QString::number(config.square[2].y());
    conf += ", ";
    
    conf += QString::number(config.square0[0].x());
    conf += ", ";
    
    conf += QString::number(config.square0[0].y());
    conf += ", ";
    
    conf += QString::number(config.square0[1].x());
    conf += ", ";
    
    conf += QString::number(config.square0[1].y());
    conf += ", ";
    
    conf += QString::number(config.square0[2].x());
    conf += ", ";
    
    conf += QString::number(config.square0[2].y());
    
    QString query = "insert into configs(id, cr1x, cr1y, cr2x, cr2y, sq1x, sq1y, sq2x, sq2y, sq3x, sq3y, sq01x, sq01y, sq02x, sq02y, sq03x, sq03y) values(" + conf + ")";
    
    if(!q.exec(query)){
        return db.lastError();
    }
    
    return QSqlError();
}

QSqlError DB::addData(const QVector<double>& pix, const QVector<double>& press)
{

    if(!db.isOpen()){
        if(!db.open()){
            return db.lastError();
        }
    }
    
    db.setDatabaseName("./sensors.db");
    
    QSqlQuery q(db);
    QString data = "100, \"";
    for(int i = 0; i < pix.size(); ++i){
        data += QString::number(pix[i]) + " " + QString::number(press[i]) + " " ;
    }
    
    data += "\"";
    
    QString query = "insert into datum(id, data) values(" + data + ")";
    
    if(!q.exec(query)){
        return db.lastError();
    }
    
    return QSqlError();
}

QStringList DB::getSensorNames()
{
    QStringList l;
    QString query = "select * from sensors";
    QSqlQuery q(query);
    int fn = q.record().indexOf("name");
    while(q.next()){
        l << q.value(fn).toString();
    }
    return l;
}

std::unique_ptr<DB> DB::instance;
std::once_flag DB::onceFlag;

DB &DB::getInstance()
{
    std::call_once(onceFlag, []{ instance.reset(new DB); instance.get()->init();});
    return *instance.get();
}

