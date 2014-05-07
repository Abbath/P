#ifndef VPROCESSOR_HPP
#define VPROCESSOR_HPP


#include <QRunnable>
#include <QMessageBox>
#include <QThread>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <helpers.hpp>

class Vprocessor : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Vprocessor(QObject* parent = 0);
    void setFilename(QString _filename){filename = _filename;}
    void setThreshold(int _threshold){threshold = _threshold;}
    void setRect(QRect _rect){rect = _rect;}
    void run();
    QImage IplImage2QImage(const IplImage *iplImage);
    QPair<int, double> processImage(QImage _image);
signals:
    void frameChanged(QImage frame);
    void maxMinBounds(QRect rect);
    void graphL(const QVector<int> &v);
    void graphM(const QVector<double> &v);
public slots:
    void stopThis();
private:
    unsigned int qrgbToGray(QRgb rgb);
private:
    QString filename;
    QRect rect;
    Config conf;
    unsigned int threshold = 255;
    volatile bool stop = false;
};
#endif // VPROCESSOR_HPP
