#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <QObject>
#include <helpers.hpp>
#include <QRunnable>
#include <QMessageBox>
#include <QThread>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <helpers.hpp>

class Processor : public QObject
{
    Q_OBJECT
    
public:
    explicit Processor(QObject *parent = 0);
    QImage loadImage(const QString &name);
    void saveImage(const QString &name);
    unsigned searchTheLight(const QImage& image, unsigned tre, unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    void run(bool vu_flag);
    //void switchMode();
    //void setMode(Modes m) {mode = m;}
    void calibrate(const QString &name, const QString &named, const QStringList &names);
    void saveResults(const QString &name);
    void getFrame(int n);
    void processVideo();
    int openVideo(const QString &name);
    //void stop() { cont = false;}
    void setDisplay(const Display& dis);
    void setFileNameV(QString _f){fileNameV = _f;}
    Display getDisplay();
    Image& getImage(){ return images[curr];}
    const QVector<double>& getRes() const {return vres;}
    const QVector<double>& getPol() const {return vres0;}
    const QString getVName() const { return fileNameV;}    

signals:
    void Update(Display dis);
    void somethingWentWrong(QString, QString);
    void plot(DataType t, QVector<double> res);
    void plot(QVector<double>, QVector<double>);
public slots:
    void align(bool vu_flag = true);
    void reset();
    void autorun(bool vu_flag = true);
    void run();
    void prev();
    void next();
    void loadConf(const QString &name);
    void saveConf(const QString &name, bool def);
    void loadData(const QString &name);
    void saveData(const QString &name);
    //void setDisplay(Display dis);
    void openImage(const QStringList &names);
    void die();

private:
    std::pair<long double,long double> leastsquares(const QVector<double> &x, const QVector<double> &yy) const;
    double calculate(const QVector<double> &res, const QVector<double> &pres, double val) const;
    QImage sharpen(const QImage& im );
    void repaint();
private:
    QVector<Image> images;
    unsigned curr = 0, frame_num = 0;
    QPoint origin[2] = {{0,0},{0,0}};
    QString fileNameV;
    QStringList fileNames;
    double sum = 0;
    //QVector<Line> lines;
    Config conf;

    QVector<double> vres;
    QVector<double> vres0;

    QVector<double> pres;
    QVector<double> res;

    QVector<double> res4[4];
    bool vid = false;
    bool proc = false;
    unsigned int threshold = 255;    
    volatile bool stop = false;
    QImage IplImage2QImage(const IplImage *iplImage);
};

#endif // PROCESSOR_HPP
