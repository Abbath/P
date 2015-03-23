#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <QObject>
#include <QtCore>

class Calculator : public QObject
{
    Q_OBJECT
public:
    explicit Calculator(QObject *parent = 0);
    ~Calculator();
    double calculate(const QVector<double> &preparedPixels, const QVector<double> &preparedPressures, double val) const;    
signals:
    
public slots:
    
private:
    std::pair<long double,long double> leastsquares(const QVector<double> &x, const QVector<double> &yy) const;
};

#endif // CALCULATOR_HPP
