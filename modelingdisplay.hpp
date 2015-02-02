#ifndef MODELINGDISPLAY_HPP
#define MODELINGDISPLAY_HPP

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

class ModelingDisplay : public QWidget
{
    Q_OBJECT
public:
    explicit ModelingDisplay(QWidget *parent = 0);
    void paintEvent(QPaintEvent *e);
signals:
    
public slots:
    void setImage(QImage _image);
private:
    QImage image;
};

#endif // MODELINGDISPLAY_HPP
