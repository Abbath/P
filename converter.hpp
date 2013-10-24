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
private:
    QImage IplImage2QImage(const IplImage *iplImage);
};

#endif // CONVERTER_HPP
