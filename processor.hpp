#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <QObject>
#include <helpers.hpp>

class Processor : public QObject
{
    Q_OBJECT
<<<<<<< HEAD
    QVector<Image> images;
    unsigned curr = 0, frame_num = 0;
    QPoint origin[2] = {{0,0},{0,0}};
    QString fileName, fileNameV;
    std::vector<std::string> fileNames = {""};
    bool vid = false;
    double sum = 0;
    //QVector<Line> lines;
    Config conf;
    
    QVector<double> vres;
    QVector<double> vres0;
    
    QVector<double> pres;
    QVector<double> res;
    
    QVector<double> res4[4];
=======
>>>>>>> b1ab59f05675701a3fd54cf8420a16f90efe4b53
    
public:
    explicit Processor(QObject *parent = 0);
    QImage loadImage(const QString &name);
    void saveImage(const QString &name);
    unsigned searchTheLight(const QImage& image, unsigned tre, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    //void switchMode();
    //void setMode(Modes m) {mode = m;}
    void calibrate(const QString &name, const QStringList &names);
    void saveResults(const QString &name);
    void getFrame(int n);
    void processVideo();
    int openVideo(const QString &name);
    //void stop() { cont = false;}
<<<<<<< HEAD
=======

>>>>>>> b1ab59f05675701a3fd54cf8420a16f90efe4b53
    const QVector<double>& getRes() const {return vres;}
    const QVector<double>& getPol() const {return vres0;}
    const QString getVName() const { return fileNameV;}    

signals:
    void Update(Display dis);
    void somethingWentWrong(QString, QString);

public slots:
    void align();
    void reset();
    void autorun();
    void run();
    void prev();
    void next();
    void loadConf(const QString &name);
    void saveConf(const QString &name, bool def);
    void loadData(const QString &name);
    void saveData(const QString &name);
    void setDisplay(Display dis);
    void openImage(const QStringList names);

private:
    std::pair<long double,long double> leastsquares(const QVector<double> &x, const QVector<double> &yy) const;
    double calculate(const QVector<double> &res, const QVector<double> &pres, double val) const;
    QImage sharpen(const QImage& im );
    void repaint();

private:
    QVector<Image> images;
    unsigned curr = 0, frame_num = 0;
    QPoint origin[2] = {{0,0},{0,0}};
    QString fileName, fileNameV;
    QStringList fileNames;
    double sum = 0;
    //QVector<Line> lines;
    Config conf;

    QVector<double> vres;
    QVector<double> vres0;

    QVector<double> pres;
    QVector<double> res;

    QVector<double> res4[4];

};

#endif // PROCESSOR_HPP
