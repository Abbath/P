#include "converter.hpp"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <fstream>
#include <limits>
#include <cassert>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

Converter::Converter()
{
}

std::pair<long double, long double> Converter::leastsquares(const QVector<double> &x, const QVector<double> &yy)const
{
    QVector<double> y = yy;
    for (int i = 0; i < y.size(); ++i) {
       /* if(qFuzzyCompare(y[i],0.0)){
            y[i]+= std::numeric_limits<double>::epsilon();

        }*/
        y[i]+=1.0;
    }
    long double A,B,a=0,b=0, at=0, tt = 0, bt =0, tmp=0;
    for(int i = 0; i < x.size(); ++i){
        tmp += x[i]*x[i]*y[i];
    }

    a = tmp;
    tmp = 0;

    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*log(y[i]);
    }

    a *= tmp;
    tmp = 0;

    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*x[i];
    }

    at = tmp;
    tmp = 0;

    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*x[i]*log(y[i]);
    }

    at *= tmp;
    tmp = 0;

    a -= at;

    for(int i = 0; i < x.size(); ++i){
        tmp += y[i];
    }

    at = tmp;
    tmp = 0;

    for(int i = 0; i < x.size(); ++i){
        tmp += x[i]*x[i]*y[i];
    }

    tt = at*tmp;
    tmp = 0;

    for(int i = 0; i < x.size(); ++i){
        tmp += x[i]*y[i];
    }

    tt -= tmp*tmp;
    tmp = 0;

    a /= tt;

    A = exp(a);

    for(int i = 0; i < x.size(); ++i){
        tmp += y[i];
    }

    b = tmp;
    tmp = 0;

    for(int i = 0; i < x.size(); ++i){
        tmp += x[i]*y[i]*log(y[i]);
    }

    b *= tmp;
    tmp = 0;

    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*x[i];
    }

    bt = tmp;
    tmp = 0;

    for(int i = 0; i < x.size(); ++i){
        tmp += y[i]*log(y[i]);
    }

    bt *= tmp;
    tmp = 0;

    b -= bt;

    b /= tt;

    B = b;

    return std::make_pair(A,B);
}

void Converter::rotate(QVector<Line> &l, double angle_x, double angle_y, double angle_z){
    Point origin = {0, 0, 0};
    for(auto it = l.begin();it != l.end();++it){
        double xt = it->x1;double yt = it->y1;double zt = it->z1;
        if(fabs(angle_x)  > std::numeric_limits<double>::epsilon()){
            it->y1 = cos(angle_x)*(yt-origin.y) - sin(angle_x)*(zt-origin.z);
            it->z1 = sin(angle_x)*(yt-origin.y) + cos(angle_x)*(zt-origin.z);
        }
        xt = it->x1; yt = it->y1; zt = it->z1;
        if(fabs(angle_y)  > std::numeric_limits<double>::epsilon()){
            it->x1 = cos(angle_y)*(xt-origin.x) + sin(angle_y)*(zt-origin.z);
            it->z1 = -sin(angle_y)*(xt-origin.x) + cos(angle_y)*(zt-origin.z);
        }
        xt = it->x1; yt = it->y1; zt = it->z1;
        if(fabs(angle_z)  > std::numeric_limits<double>::epsilon()){
            it->x1 = cos(angle_z)*(xt-origin.x) - sin(angle_z)*(yt-origin.y);
            it->y1 = sin(angle_z)*(xt-origin.x) + cos(angle_z)*(yt-origin.y);
        }

        xt = it->x2;yt = it->y2; zt = it->z2;
        if(fabs(angle_x)  > std::numeric_limits<double>::epsilon()){
            it->y2 = cos(angle_x)*(yt-origin.y) - sin(angle_x)*(zt-origin.z);
            it->z2 = sin(angle_x)*(yt-origin.y) + cos(angle_x)*(zt-origin.z);
        }
        xt = it->x2; yt = it->y2; zt = it->z2;
        if(fabs(angle_y)  > std::numeric_limits<double>::epsilon()){
            it->x2 = cos(angle_y)*(xt-origin.x) + sin(angle_y)*(zt-origin.z);
            it->z2 = -sin(angle_y)*(xt-origin.x) + cos(angle_y)*(zt-origin.z);
        }
        xt = it->x2; yt = it->y2; zt = it->z2;
        if(fabs(angle_z)  > std::numeric_limits<double>::epsilon()){
            it->x2 = cos(angle_z)*(xt-origin.x) - sin(angle_z)*(yt-origin.y);
            it->y2 = sin(angle_z)*(xt-origin.x) + cos(angle_z)*(yt-origin.y);
        }
    }
}

QVector<Line> Converter::convert(const QImage &image, Modes mode/*, int left, int top, int right, int bottom*/){
    QVector<Line> result;
    int left = 0,top = 0,right = image.width(),bottom = image.height();
    for( int i = left; i < right; ++i){
        for( int j = top; j < bottom; ++j){
            Line p;
            p.x1 = p.x2 = i;
            p.y1 = p.y2 = j;
            p.z1 = qGray(image.pixel(i,j));
            p.c = p.z1;
            QVector<int> v;
            if(i!=left) v.push_back(qGray(image.pixel(i-1,j)));
            if(i < right-1) v.push_back(qGray(image.pixel(i+1,j)));
            if(j!=top) v.push_back(qGray(image.pixel(i,j-1)));
            if(j < bottom-1) v.push_back(qGray(image.pixel(i,j+1)));
            if(i!=left && j!= top) v.push_back(qGray(image.pixel(i-1,j-1)));
            if(i < right-1 && j!=top) v.push_back(qGray(image.pixel(i+1,j-1)));
            if(j < bottom-1 && i!=left) v.push_back(qGray(image.pixel(i-1,j+1)));
            if(i < right-1 && j < bottom-1) v.push_back(qGray(image.pixel(i+1,j+1)));
            int min = *(std::min_element(v.begin(),v.end()));
            if(min < qGray(image.pixel(i,j))){
                p.z2 = p.z1 - min;
            }else{
                p.z2 = p.z1;
            }
            result.push_back(p);
        }
    }
    switch (mode) {
    case ISO:
        rotate(result, 3.1415/180*35.2,3.1415/4,-3.1415/4);
        break;
    case BOTTOM:
        rotate(result, 3.1415/180*90,0,0);
        break;
    case LEFT:
        rotate(result, 3.1415/180*90,0,0);
        rotate(result, 0, 3.1415/180*90,0);
        break;
    case RIGHT:
        rotate(result, 3.1415/180*90,0,0);
        rotate(result, 0, -3.1415/180*90,0);
        break;
    default:
        break;
    }
    return result;
}

bool saveImageToFile(QImage image, const QString filename){
    return image.save(filename, "BMP", 100);
}

int Converter::processVideo(QString s)
{
    QTime timer;
    timer.start();
    QList<QFuture<bool>> fv;
    CvCapture * capture = cvCaptureFromAVI(s.toStdString().c_str());
    if(!capture)
    {
        return 0;
    }
    //int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    // qDebug() << "* FPS: %d" <<  fps << "\n";
    IplImage* frame = NULL;
    int frame_number = 0;
    while ((frame = cvQueryFrame(capture))) {
        char filename[100];
        strcpy(filename, "frame_");
        char frame_id[30];
        sprintf(frame_id,"%d",frame_number);
        strcat(filename, frame_id);
        strcat(filename, ".bmp");
        QImage img = IplImage2QImage(frame).mirrored(false, true);
        QFuture<bool> fb = QtConcurrent::run(&saveImageToFile, img, QString(filename));
        fv.append(fb);
        frame_number++;
    }
    cvReleaseCapture(&capture);
    foreach (auto& x, fv) {
        x.result();
    }
    std::cerr << "time: " << timer.elapsed() << std::endl;
    return frame_number;
}

double Converter::calculate(const QVector<double> &res, const QVector<double> &pres, double val) const 
{

    /*double sum = 0.0,x1 = 0.0,y1 = 0.0,y0=0.0,x0=0.0;

    for(auto it = res.begin(); it != res.end(); ++it){
        if(*it > val){
            if(it == res.begin()){
                x0 = *res.begin();
                y0 = *pres.begin();
                x1 = *(res.begin()+1);
                y1 = *(pres.begin()+1);
                break;
            }
            x1 = *it;
            y1 = pres[it - res.begin()];
            x0 = *(it-1);
            y0 = pres[it - res.begin() - 1];
            break;
        }
        if(it + 1 == res.end()){
            x0 = *(it-1);
            y0 = pres[res.size()-2];
            x1 = *it;
            y1 = pres[res.size()-1];
        }
    }
    sum = y0 + (y1 - y0) * (val - x0) / ( x1 - x0);*/

    /*double sum=0,l=1;

    for(int j = 0;j < res.size(); ++j){
        for(int i = 0; i < res.size(); ++i){
            if( j != i){
                l*= (val - res[i])/(double)(res[j]-res[i]);
            }
        }
        sum += l*pres[j];
        l = 1;
    }*/
    
    auto p = leastsquares(res,pres);
    double sum = p.first * exp(p.second*val)-1.0;
    return sum;

}

QImage Converter::IplImage2QImage(const IplImage *iplImage)
{
    int height = iplImage->height;
    int width = iplImage->width;

    const uchar *qImageBuffer =(const uchar*)iplImage->imageData;
    QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
    return img.rgbSwapped();
}

QVector<int> Converter::dbscan(QVector<QPoint> &v)
{
    const int n = v.size();
    int L_min = 10;
    int kol = 5;
    std::fstream f("dbscan.cfg");
    f >> L_min >> kol;
    QVector<QVector<int>> L;
    QVector<int> rez;
    QVector<int> marked;
    QVector<int> group;
    rez.resize(n);
    L.resize(n);
    marked.resize(n);
    group.resize(n);
    for(QVector<int> &x : L ){
        x.resize(n);
    }
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < n; ++j){
            L[i][j] = dist(v[i],v[j]) <= L_min;
            if(L[i][j]){
                rez[i]++;
            }
        }
        for(int j = 0; j < n; j++){
            L[i][j] *= j;
        }
    }
    int c = 1;
    for(int i = 0; i < n; ++i){
        if(marked[i] == 0){
            if(rez[i] < kol){
                marked[i] = 1;
                group[i] = -1;
            }else{
                auto uvec = group;
                group[i] = c;
                while(!std::equal(uvec.begin(),uvec.end(),group.begin())){
                    uvec = group;
                    for(int j = 0; j < n; ++j){
                        if(group[j] == c && marked[j] == 0){
                            if(rez[i] < kol){
                                marked[j] = 1;
                                group[j] = -1;
                            }else{
                                marked[j] = 1;
                                for(int k = 0; k < n; ++k){
                                    if(L[j][k] > 0){
                                        group[k] = c;
                                    }
                                }
                            }
                        }
                    }
                }
                c++;
            }
        }else{
            continue;
        }
    }
    return group;
}

