#ifndef IMAGEAREA_HPP
#define IMAGEAREA_HPP

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QFileDialog>
#include <QMessageBox>
#include <QVector>
#include <helpers.hpp>
#include <converter.hpp>
#include <QtMultimediaWidgets/QVideoWidget>
namespace Ui {
class ImageArea;
}

class ImageArea : public QWidget
{
    Q_OBJECT
    Image* images;
    unsigned curr = 0, frame_num = 0;
    QPoint origin[2] = {{0,0},{0,0}};
    QString fileName, fileNameV;
    QStringList fileNames;
    double sum = 0,GY=25,YR=35;
    QPoint zoom;
    QVector<Line> lines;
    QVector<QPoint> lasts[4], hull[4];
    QVector<int> dbs[4];
    Converter conv;
    bool rect = false,zoom_b = false,d3 = false, vid = false;
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
    explicit ImageArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    ~ImageArea();

    void loadImage();
    void saveImage();

    void loadConf(bool def);
    void saveConf(bool def);

    void loadData();
    void saveData();

    unsigned searchTheLight(unsigned x1, unsigned y1, unsigned x2, unsigned y2);

    void switchMode();
    void setMode(Modes m) {mode = m;}

    void autorun();
    void calibrate();

    void saveResults();

    void getFrame(int n);
    void processVideo();

public slots:

    void openImage();
    int openVideo();

    void align();
    void reset();

    void run();
    void prev();
    void next();
    void stop() { cont = false;}

    void setGY(double val){GY = val;}
    void setYR(double val){YR = val;}
    void setThreshold(int val){ images[curr].threshold = val;}

    const QVector<double>& getRes() const {return vres;}
    const QVector<double>& getPol() const {return vres0;}
    const QString getVName() const { return fileNameV;}
private slots:
    void checkVideoProcess();
private:
    void sharpen();
    Ui::ImageArea *ui;
    int tre() const { return images[curr].threshold; }
signals:
    void imageUpdated(QString filename);
    void giveFramesNumber(int n);
    void giveImage(Image im);
    void imageChanged(QString filename);
};

#endif // IMAGEAREA_HPP
