#ifndef MODELINGCORE_HPP
#define MODELINGCORE_HPP

#include <QObject>
#include <QtCore>
#include <QtGui>
#include <iostream>
#include <atomic>
#include <fstream>
#include <vector>
#include <functional>
#include <algorithm>
#include <utility>
#include <set>
#include <tuple>
#include <cassert>
#include <chrono>
#include <random>
#include <numeric>
#include <omp.h>
#include "helpers.hpp"

struct Point{
    double x;
    double y;
    double z;
    Point operator+ (const Point& a){
        return {this->x+a.x, this->y+a.y, this->z+a.z};
    }
};
typedef Point Vector;
typedef std::vector<std::tuple<Point, Point, Point, Point>> Grid;



class ModelingCore : public QObject
{
    Q_OBJECT
    
public:
    explicit ModelingCore(QObject *parent = 0);
    void run();
    int getRay_number() const;
    void setRay_number(int value);
    
    double getDie_size() const;
    void setDie_size(double value);
    
    double getMem_size() const;
    void setMem_size(double value);
    
    double getMargin() const;
    void setMargin(double value);
    
    QString getFilename() const{return filename;}
    void setFilename(const QString &value){filename = value;}
    
    void saveImage(QString _filename){
        QImageWriter wr(_filename);
        if(wr.write(im)/*im.save(_filename)*/){
            emit saved(true);
        }else{
            emit error(wr.errorString());
            emit saved(false);
        }
    }
    double getPressure() const;
    void setPressure(double value);
    
    double getSpacer_height() const;
    void setSpacer_height(double value);
    
    double getThickness() const;
    void setThickness(double value);
    
    QImage getIm() const;
    void setIm(const QImage &value);
    
    double getWavelength() const;
    void setWavelength(double value);
    
    double getX_angle() const;
    void setX_angle(double value);
    
    double getY_angle() const;
    void setY_angle(double value);
    
    bool getIs_data_external() const;
    void setIs_data_external(bool value);
    
    double getCam_angle_x() const;
    void setCam_angle_x(double value);
    
    double getCam_angle_y() const;
    void setCam_angle_y(double value);
    
    int getN() const;
    void setN(int value);
    
    double getYm() const;
    void setYm(double value);
    
    double getPr() const;
    void setPr(double value);
    
    int getHoles_rows_number() const;
    void setHoles_rows_number(int value);
    
    int getSpace_size() const;
    void setSpace_size(int value);
    
    int getHole_size() const;
    void setHole_size(int value);
    
    void setData(const ModelingData& data);
    
    void calculateStress(std::tuple<Point, Point, Point> three);
    int getScale_coef() const;
    void setScale_coef(int value);
    
private:
    double w(double x, double y);
    std::vector<Point> readFile(std::string filename);
    double dist(const Point &a, const Point &b);
    double square(const Point& a, const Point& b,const Point& c);
    std::tuple<Point, Point, Point> closest_external(const Point &p, const std::vector<Point> &v);
    std::tuple<Point, Point, Point> closest_internal(const Point &p);    
    Vector normal(std::tuple<Point, Point, Point> points);
    double angle(const Vector &a, const Vector &b);
    Vector reflect(const Vector &v, const Vector &n);
    double z(double x, double y, std::tuple<Point, Point, Point> closest_external);
    std::tuple<double, double> test(const Point &p/*, const std::vector<Point> &data*/);
    bool check_holes(int x, int y);
    bool check_triangle(const Point& p, const std::tuple<Point, Point, Point>& closest_external);
    Grid build_grid();
    QImage rotate(const QImage& im);
    std::pair<bool, Vector> check_grid(const Point &origin, const Vector &v, int y, int x0, int y0);
    QColor waveLengthToRGB(double wavelength);
signals:
    void saved(bool);
    void lil(int);
    void sendImage(QImage image);
    void error(QString s);
public slots:
    void Stop();
private:
    
    QImage im;
    QPixmap pixx;
    QString filename;
    std::vector<Point> data;
    bool is_data_external = false;
    int ray_number;
    int scale = 1000000;
    int scale_coef = 10;
    int hole_size = 60;
    int space_size = 340;
    int holes_rows_number = 6;
    int N = 10;
    int hole_space_size = hole_size+space_size;
    int some_strange_size;
    double die_size;
    double mem_size;
    double margin;
    double pressure;
    double thickness;
    double spacer_height;
    double wavelength;
    double x_angle;
    double y_angle;
    double cam_angle_x;
    double cam_angle_y;
    double clean_space_f;
    double clean_space_b;
    double ym;
    double pr;
    std::atomic_bool stop;
    std::pair<double, double> stress(double x , double y, double z);
    double w2(double x, double y);
};

#endif // MODELINGCORE_HPP
