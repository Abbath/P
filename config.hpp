#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <QObject>
#include <array>
#include <QtCore>

typedef std::array<QPoint, 3> Square;

class Config
{
public:
    explicit Config();
    ~Config();
    bool saveConf(const QString& name);
    bool loadConf(const QString& name);
    QRect getCrop() const;
    QRect& getCropRef() {return crop;}
    
    void setCrop(const QRect &value);
    
    Square getSquare() const;
    Square& getSquareRef() { return square; }
    
    void setSquare(const Square &value);
    
    Square getSquare0() const;
    Square& getSquare0Ref() {return square0;}
    
    void setSquare0(const Square &value);
    
signals:
    
public slots:
    
private:
    QRect crop;
    Square square;
    Square square0;
};

#endif // CONFIG_HPP
