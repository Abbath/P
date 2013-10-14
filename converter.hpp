#ifndef CONVERTER_HPP
#define CONVERTER_HPP
#include <QtCore>
#include <QtGui>
#include <helpers.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/math/tools/solve.hpp>

class Converter
{
public:
    Converter();
    void rotate(QVector<Line> &l, double angle_x =0, double angle_y =0, double angle_z =0);
    QVector<Line> convert(QImage &image, Modes mode);
    int processVideo(QString s);
    double calculate(QVector<int> &res, QVector<double> &pres, int val);
};

#endif // CONVERTER_HPP
