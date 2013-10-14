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
    QImage image;
    QString fileName, fileNameV;
    QPoint square[3];
    unsigned counter = 0;
    QPoint crop[2];
    QPoint zoom;
    QVector<Line> lines;
    Converter conv;
    bool rect = false,zoom_b = false,d3 = false,interf = false;
    unsigned threshold = 128;
    unsigned bound_counter[4] = {0,0,0,0};
    Modes mode = ISO;
    QVector<QRect> randrect;
    Config conf;
    QVector<double> pres;
    QVector<int> res;
public:
    explicit ImageArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    ~ImageArea();
    void load();
    unsigned searchTheLight(unsigned x1, unsigned y1, unsigned x2, unsigned y2);
    void switchMode();
    void saveImage();
    void saveConf();
    void loadConf();
    void setMode(Modes m) {mode = m;}
 // void randomgen();
    void autorun();
    void calibrate();
    void getFrame(int n);
public slots:
    void align();
    void open();
    int openVideo();
    void reset();
    void run();
private:
    void sharpen();
    Ui::ImageArea *ui;
    int tre() const { return threshold; }
};

#endif // IMAGEAREA_HPP
