#ifndef CONVERTER_HPP
#define CONVERTER_HPP
#include <QtCore>
#include <QtGui>
#include <QGridLayout>
#include <QProgressBar>
#include <helpers.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/math/tools/solve.hpp>
#include <functional>
#include <memory>
#include <tuple>
class Converter
{
public:
    Converter();
    long double sum(const QVector<double> &x, const QVector<double>&y,std::function<long double (double, double)> f );
    std::pair<long double,long double> leastsquares(const QVector<double> &x, const QVector<double> &y);
    void rotate(QVector<Line> &l, double angle_x =0, double angle_y =0, double angle_z =0);
    QVector<Line> convert(QImage &image, Modes mode);
    int processVideo(QString s);
    double calculate(QVector<double> &res, QVector<double> &pres, double val);
    QVector<QPoint> gethull(QVector<QPoint> _p);
    QPoint mid(QVector<QPoint> v);
    QVector<int> dbscan(QVector<QPoint> &v);
private:

    const int N = 3;
    QVector<QPoint> p;
    QVector<int> belongs_to_hull;
    QVector<int> pt;
    QImage IplImage2QImage(const IplImage *iplImage);
    void qh(QVector<int> pt, int n);
    int pivotize(QVector<int> pt, int n);
    void inithull(QVector<int> pt, int n, int &minx, int &maxx);
    QVector<int> delete_right(QVector<int> pt, int &num, int p1, int p2);
    double dist(QPoint &a, QPoint &b){
        return sqrt((a.x()-b.x())*(a.x()-b.x()) + (a.y() - b.y())*(a.y()-b.y()));
    }

    int cross (int pp,int a,int b){
        return  ((p[a].x()-p[pp].x())*(p[b].y()-p[pp].y()) - (p[a].y()-p[pp].y())*(p[b].x()-p[pp].x()));
    }
    int leftturn (int a,int b,int c) {
        return (cross(c,a,b)>0.0);
    }
};

#endif // CONVERTER_HPP
