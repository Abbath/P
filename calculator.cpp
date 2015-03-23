#include "calculator.hpp"

Calculator::Calculator(QObject *parent) : QObject(parent)
{
    
}

Calculator::~Calculator()
{
    
}

double Calculator::calculate(const QVector<double> &preparedPixels, const QVector<double> &preparedPressures, double val) const
{
    auto p = leastsquares(preparedPixels, preparedPressures);
    double sum = p.first * exp(p.second * val) - 1.0;
    return sum;
}

std::pair<long double, long double> Calculator::leastsquares(const QVector<double> &x, const QVector<double> &yy) const
{
    QVector<double> y = yy;
    for (int i = 0; i < y.size(); ++i) {
        y[i] += 1.0;
    }
    long double A, B, a = 0, b = 0, at = 0, tt = 0, bt = 0, tmp = 0;
    for (int i = 0; i < x.size(); ++i) {
        tmp += x[i] * x[i] * y[i];
    }
    
    a = tmp;
    tmp = 0;
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * log(y[i]);
    }
    
    a *= tmp;
    tmp = 0;
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * x[i];
    }
    
    at = tmp;
    tmp = 0;
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * x[i] * log(y[i]);
    }
    
    at *= tmp;
    tmp = 0;
    
    a -= at;
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i];
    }
    
    at = tmp;
    tmp = 0;
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += x[i] * x[i] * y[i];
    }
    
    tt = at * tmp;
    tmp = 0;
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += x[i] * y[i];
    }
    
    tt -= tmp * tmp;
    tmp = 0;
    
    a /= tt;
    
    A = exp(a);
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i];
    }
    
    b = tmp;
    tmp = 0;
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += x[i] * y[i] * log(y[i]);
    }
    
    b *= tmp;
    tmp = 0;
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * x[i];
    }
    
    bt = tmp;
    tmp = 0;
    
    for (int i = 0; i < x.size(); ++i) {
        tmp += y[i] * log(y[i]);
    }
    
    bt *= tmp;
    tmp = 0;
    
    b -= bt;
    
    b /= tt;
    
    B = b;
    
    return std::make_pair(A, B);
}

