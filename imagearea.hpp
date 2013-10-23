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
#include <algorithm>
namespace Ui {
class ImageArea;
}

class ImageArea : public QWidget
{
    Q_OBJECT
    Image* images;
    unsigned curr = 0, frame_num = 0;
    //QImage image;
    QString fileName, fileNameV;
    QStringList fileNames;
    //QPoint square[3];
    //unsigned counter = 0;
    double sum = 0,GY=25,YR=35;
    //QPoint crop[2];
    QPoint zoom;
    QVector<Line> lines;
    Converter conv;
    bool rect = false,zoom_b = false,d3 = false;
    //unsigned threshold = 128;
    //unsigned bound_counter[4] = {0,0,0,0};
    Modes mode = ISO;
    //QVector<QRect> randrect;
    Config conf;
    QVector<double> vres;
    QVector<double> vres0;
    QVector<double> pres;
    QVector<double> res;
public:
    explicit ImageArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *e);
 //   void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    ~ImageArea();
    void loadImage();
    unsigned searchTheLight(unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    void switchMode();
    void saveImage();
    void saveConf(bool def);
    void loadConf(bool def);
    void setMode(Modes m) {mode = m;}
    void autorun();
    void calibrate();
    void getFrame(int n);
    void processVideo();
    void loadData();
    void saveData();
public slots:
    void align();
    void openImage();
    int openVideo();
    void reset();
    void run();
    void prev();
    void next();
    void setGY(double val){GY = val;}
    void setYR(double val){YR = val;}
    const QVector<double>& getRes() const {return vres;}
    const QVector<double>& getPol() const {return vres0;}
private:
    void sharpen();
    Ui::ImageArea *ui;
    int tre() const { return images[curr].threshold; }
signals:
    void giveImage(Image im);
};

#endif // IMAGEAREA_HPP
