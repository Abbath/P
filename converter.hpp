#ifndef CONVERTER_HPP
#define CONVERTER_HPP
#include <QtCore>
#include <QtGui>
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
    long double sum(const QVector<int> &x, const QVector<int>&y,std::function<long double (int, int)> f );
    std::pair<long double,long double> leastsquares(const QVector<int> &x, const QVector<int>&y);
    void rotate(QVector<Line> &l, double angle_x =0, double angle_y =0, double angle_z =0);
    QVector<Line> convert(QImage &image, Modes mode);
    int processVideo(QString s);
    double calculate(QVector<int> &res, QVector<double> &pres, int val);
private:
    QImage IplImage2QImage(const IplImage *iplImage);
};

#endif // CONVERTER_HPP
