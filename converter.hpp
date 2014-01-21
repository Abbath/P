#ifndef CONVERTER_HPP
#define CONVERTER_HPP
#include <QtCore>
#include <QtGui>
#include <QGridLayout>
#include <QProgressBar>
#include <helpers.hpp>
#include <functional>
#include <memory>
#include <tuple>
class Converter
{
public:

    Converter();
    std::pair<long double,long double> leastsquares(const QVector<double> &x, const QVector<double> &yy) const;
    void rotate(QVector<Line> &l, double angle_x =0, double angle_y =0, double angle_z =0);
    QVector<Line> convert(const QImage &image, Modes mode);
    int processVideo(QString s);
    double calculate(const QVector<double> &res, const QVector<double> &pres, double val) const;
    QVector<int> dbscan(QVector<QPoint> &v);
private:

    QVector<QPoint> p;
    QVector<int> belongs_to_hull;
    QVector<int> pt;
    QImage IplImage2QImage(const IplImage *iplImage);
    double dist(QPoint &a, QPoint &b){
        return sqrt((a.x()-b.x())*(a.x()-b.x()) + (a.y() - b.y())*(a.y()-b.y()));
    }
};

#endif // CONVERTER_HPP
