#include "db.hpp"

DB::DB()
{
    
}

QPair<QVector<int>, QVector<int>> DB::getCDIDs()
{
    QSqlQuery q("select id from configs");
    //q.next();
    QVector<int> cids, dids;
    while(q.next()){
        cids.push_back(q.value(0).toInt());
    }
    
    QSqlQuery q0("select id from datum");
    //q.next();
    while(q0.next()){
        dids.push_back(q0.value(0).toInt());
    }
    
    return QPair<QVector<int>, QVector<int>> (cids, dids);
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

QSqlError DB::addFullSensor(QString name, const Sensor &sensor)
{
    QSqlError err = addSensor(name, sensor.getModelingData());
    if(err.isValid()){
        return db.lastError();
    }
    err = addConf(sensor.getConf());
    if(err.isValid()){
        return db.lastError();
    }
    err = addData(sensor.getMeasurementData().first, sensor.getMeasurementData().second);
    if(err.isValid()){
        return db.lastError();
    }
    return QSqlError();
}

QSqlError DB::addSensor(QString name, const ModelingData &data)
{
    //    if(!db.isOpen()){
    //        if(!db.open()){
    //            return db.lastError();
    //        }
    //        db.setDatabaseName("./sensors.db");
    //    }
    auto ids = getCDIDs();
    
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
    
    for(int i = 0; true; i++){
        if(!ids.first.contains(i)){
            confId = i;
            break;
        }
    }
    sensor += QString::number(confId);
    sensor += ", ";
    
    for(int i = 0; true; i++){
        if(!ids.second.contains(i)){
            dataId = i;   
            break;
        }
    }
    
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
    QString conf = QString::number(confId) + ", ";
    
    //    conf += "\"" + name + "\"";
    //    conf += ", ";
    
    conf += QString::number(config.getCrop().left());
    conf += ", ";
    
    conf += QString::number(config.getCrop().top());
    conf += ", ";
    
    conf += QString::number(config.getCrop().right());
    conf += ", ";
    
    conf += QString::number(config.getCrop().bottom());
    conf += ", ";
    
    conf += QString::number(config.getSquare()[0].x());
    conf += ", ";
    
    conf += QString::number(config.getSquare()[0].y());
    conf += ", ";
    
    conf += QString::number(config.getSquare()[1].x());
    conf += ", ";
    
    conf += QString::number(config.getSquare()[1].y());
    conf += ", ";
    
    conf += QString::number(config.getSquare()[2].x());
    conf += ", ";
    
    conf += QString::number(config.getSquare()[2].y());
    conf += ", ";
    
    conf += QString::number(config.getSquare0()[0].x());
    conf += ", ";
    
    conf += QString::number(config.getSquare0()[0].y());
    conf += ", ";
    
    conf += QString::number(config.getSquare0()[1].x());
    conf += ", ";
    
    conf += QString::number(config.getSquare0()[1].y());
    conf += ", ";
    
    conf += QString::number(config.getSquare0()[2].x());
    conf += ", ";
    
    conf += QString::number(config.getSquare0()[2].y());
    
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
    QString data = QString::number(dataId) + ", \"";
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

QSqlError DB::removeSensor(QString name)
{
    QSqlQuery q("delete from sensors where name = \"" + name + "\"");
    if(!q.exec()){
        return db.lastError();
    }
    return QSqlError();
}

ModelingData DB::getSensor(QString name, int& cid, int& did)
{
    ModelingData data;
    QSqlQuery q("select * from sensors where name = \"" + name + "\"");
    if(!q.exec()){
        qDebug() << db.lastError().text() << q.lastError().text();
        return data;
    }
    q.next();
    data.setDie_size(q.value(2).toDouble()/1000);
    data.setMem_size(q.value(3).toDouble()/1000);
    data.setMem_thickness(q.value(4).toDouble()/1000000);
    data.setSpacer_height(q.value(5).toDouble()/100);
    data.setPressure(q.value(6).toDouble()*1000/25);
    data.setWavelength(q.value(7).toInt());
    data.setX_angle(q.value(8).toDouble()/57.3);
    data.setY_angle(q.value(9).toDouble()/57.3);
    data.setCamx(q.value(10).toDouble()/57.3);
    data.setCamy(q.value(11).toDouble()/57.3);
    data.setRows(q.value(12).toInt());
    data.setHole_size(q.value(13).toInt());
    data.setSpace_size(q.value(14).toInt());
    data.setYm(q.value(15).toDouble()*1e9);
    data.setPr(q.value(16).toDouble());
    cid = q.value(17).toInt();
    did = q.value(18).toInt();
    return data;
}

Config DB::getConfig(int id)
{
    Config conf;
    QSqlQuery q("select * from configs where id = "+ QString::number(id));
    q.next();
    conf.getCropRef().setLeft(q.value(1).toInt());
    conf.getCropRef().setTop(q.value(2).toInt());
    conf.getCropRef().setRight(q.value(3).toInt());
    conf.getCropRef().setBottom(q.value(4).toInt());
    conf.getSquareRef()[0].setX(q.value(5).toInt());
    conf.getSquareRef()[0].setY(q.value(6).toInt());        
    conf.getSquareRef()[1].setX(q.value(7).toInt());
    conf.getSquareRef()[1].setY(q.value(8).toInt());
    conf.getSquareRef()[2].setX(q.value(9).toInt());
    conf.getSquareRef()[2].setY(q.value(10).toInt());
    conf.getSquare0Ref()[0].setX(q.value(11).toInt());
    conf.getSquare0Ref()[0].setY(q.value(12).toInt());
    conf.getSquare0Ref()[1].setX(q.value(13).toInt());
    conf.getSquare0Ref()[1].setY(q.value(14).toInt());
    conf.getSquare0Ref()[2].setX(q.value(15).toInt());
    conf.getSquare0Ref()[2].setY(q.value(16).toInt());
    return conf;
}

QPair<QVector<double>, QVector<double> > DB::getData(int id)
{
    QVector<double> pix, pres;
    
    QSqlQuery q("select * from datum where id = " + QString::number(id));
    q.next();
    QString data = q.value(1).toString();
    QStringList lst = data.split(QChar(' '));
    for(int i = 0; i < lst.size()-1; i+=2){
        pix.push_back(lst[i].toDouble());
        pres.push_back(lst[i+1].toDouble());
    }
    return QPair<QVector<double>, QVector<double> >(pix, pres);
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
int DB::getConfId() const
{
    return confId;
}

void DB::setConfId(int value)
{
    confId = value;
}
int DB::getDataId() const
{
    return dataId;
}

void DB::setDataId(int value)
{
    dataId = value;
}



