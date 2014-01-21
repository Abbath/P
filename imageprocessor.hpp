#ifndef IMAGEPROCESSOR_HPP
#define IMAGEPROCESSOR_HPP

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <helpers.hpp>
#include <converter.hpp>
#include <QtMultimediaWidgets/QVideoWidget>

class ImageProcessor : QObject
{
    Q_OBJECT
    Image* images;
    unsigned curr = 0, frame_num = 0;
    QPoint origin[2] = {{0,0},{0,0}};
    QString fileName, fileNameV;
    QStringList fileNames;
    double sum = 0;
   // QPoint zoom;
    QVector<Line> lines;
    QVector<QPoint> lasts[4], hull[4];
    QVector<int> dbs[4];
    Converter conv;
    bool /*rect = false,zoom_b = false*/d3 = false, vid = false;
    Modes mode = ISO;
    Config conf;
    QVector<double> vres;
    QVector<double> vres0;
    QVector<double> pres;
    QVector<double> res;
    QVector<double> res4[4];
    volatile bool cont = true;
    QFuture<int> fn;
public:
    void loadImage();
    void saveImage(const QString &name);

    void loadConf(const QString &name);
    void saveConf(const QString &name, bool def);

    void loadData(const QString &name);
    void saveData(const QString &name);

    unsigned searchTheLight(unsigned x1, unsigned y1, unsigned x2, unsigned y2);

    void switchMode();
    void setMode(Modes m) {mode = m;}

    void autorun();
    void calibrate(const QString &name, const QStringList &names);

    void saveResults(const QString &name);

    void getFrame(int n);
    void processVideo();

    void openImage(const QStringList &names);
    int openVideo(const QString &name);

    void align();
    void reset();

    void run();
    void prev();
    void next();
    void stop() { cont = false;}

    void setThreshold(int val){ images[curr].threshold = val;}

    const QVector<double>& getRes() const {return vres;}
    const QVector<double>& getPol() const {return vres0;}
    const QString getVName() const { return fileNameV;}
    ImageProcessor(QObject * parent = 0);
private: 
    void checkVideoProcess();
private:
    void sharpen();
    int tre() const { return images[curr].threshold; }
signals:
    void needRepaint();
    void somethingWentWrong(QString, QString);
    void imageUpdated(QString filename);
    void giveFramesNumber(int n);
    void giveImage(Image im);
    void imageChanged(QString filename);
};

#endif // IMAGEPROCESSOR_HPP