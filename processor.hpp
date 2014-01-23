#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <QObject>
#include <helpers.hpp>

class Processor : public QObject
{
    Q_OBJECT
    QVector<Image> images;
    unsigned curr = 0, frame_num = 0;
    QPoint origin[2] = {{0,0},{0,0}};
    QString fileName, fileNameV;
    std::vector<std::string> fileNames = {""};
    double sum = 0;
    //QVector<Line> lines;
    Config conf;
    
    QVector<double> vres;
    QVector<double> vres0;
    
    QVector<double> pres;
    QVector<double> res;
    
    QVector<double> res4[4];
    
public:
    explicit Processor(QObject *parent = 0);
    QImage loadImage(const std::string &name);
    void saveImage(const QString &name);

    void loadConf(const QString &name);
    void saveConf(const QString &name, bool def);

    void loadData(const QString &name);
    void saveData(const QString &name);

    unsigned searchTheLight(unsigned x1, unsigned y1, unsigned x2, unsigned y2);

    void switchMode();
    //void setMode(Modes m) {mode = m;}

    void autorun();
    void calibrate(const QString &name, const QStringList &names);

    void saveResults(const QString &name);

    void getFrame(int n);
    void processVideo();

    int openVideo(const QString &name);

    void align();
    void reset();

    void run();
    void prev();
    void next();
    //void stop() { cont = false;}


    const QVector<double>& getRes() const {return vres;}
    const QVector<double>& getPol() const {return vres0;}
    const QString getVName() const { return fileNameV;}    
signals:
    void Update(Display dis);
    void somethingWentWrong(QString, QString);
public slots:
    void setDisplay(Display dis);
    void openImage(const QVector<std::string> &names);
private:
    QImage sharpen(const QImage& im );
    void repaint();
};

#endif // PROCESSOR_HPP
