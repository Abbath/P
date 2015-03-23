#include "config.hpp"

Config::Config()
{
    
}

Config::~Config()
{
    
}

bool Config::saveConf(const QString &name)
{
    QFile file(name);
    if (file.open(QFile::WriteOnly)) {
        QTextStream str(&file);
        str << crop.left() << " " << crop.top() << "\n";
        str << crop.right() << " " << crop.bottom() << "\n";
        str << square[0].x() << " " << square[0].y() << "\n";
        str << square[1].x() << " " << square[1].y() << "\n";
        str << square[2].x() << " " << square[2].y() << "\n";
        str << square0[0].x() << " " << square0[0].y() << "\n";
        str << square0[1].x() << " " << square0[1].y() << "\n";
        str << square0[2].x() << " " << square0[2].y() << "\n";
        
        return true;
    }else {
        return false;
    }
}

bool Config::loadConf(const QString &name)
{
    QFile file(name);
    if (file.open(QFile::ReadOnly)) {
        QTextStream str(&file);
        int r, t, l, b;
        str >> l >> t;
        str >> r >> b;
        crop.setRight(r);
        crop.setLeft(l);
        crop.setTop(t);
        crop.setBottom(b);
        str >> square[0].rx() >> square[0].ry();
        str >> square[1].rx() >> square[1].ry();
        str >> square[2].rx() >> square[2].ry();
        str >> square0[0].rx() >> square0[0].ry();
        str >> square0[1].rx() >> square0[1].ry();
        str >> square0[2].rx() >> square0[2].ry();
        if (str.status() & QTextStream::ReadCorruptData || str.status() & QTextStream::ReadPastEnd) {
            return false;
        }
        return true;
    } else {
        return false;
    }
}
QRect Config::getCrop() const
{
    return crop;
}

void Config::setCrop(const QRect &value)
{
    crop = value;
}
Square Config::getSquare() const
{
    return square;
}

void Config::setSquare(const Square &value)
{
    square = value;
}
Square Config::getSquare0() const
{
    return square0;
}

void Config::setSquare0(const Square &value)
{
    square0 = value;
}




