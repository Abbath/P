#include "converter.hpp"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <QMessageBox>

Converter::Converter()
{
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

QVector<Line> Converter::convert(QImage &image, Modes mode/*, int left, int top, int right, int bottom*/){
    QVector<Line> result;
   /* if(left < 0) left = 0;
    if(top < 0) top = 0;
    //if(right > image.width()) right = image.width();
    //if(bottom > image.height()) bottom = image.height();
    //points.clear();
    //pix.fill(Qt::black);
    if(left > right){
        left ^= right;
        right ^= left;
        left ^= right;
    }
    if(top > bottom){
        top ^= bottom;
        bottom ^= top;
        top ^= bottom;
    }*/
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
               /* for( unsigned k = 0; k < p.c-min; ++k){
                    Point p0;
                    p0.x = i;
                    p0.y = j;
                    p0.z = qGray(image.pixel(i,j))-(k+1);
                    p0.c = qGray(image.pixel(i,j));
                    points.push_back(p0);
                }*/
                p.z2 = p.z1 - min;
            }else{
                p.z2 = p.z1;
            }
            result.push_back(p);
        }
    }
    /*origin.x = 0;
    origin.y = 0;
    origin.z = 0.0;*/
    switch (mode) {
    case ISO:
        rotate(result, 3.1415/180*35.2,3.1415/4,-3.1415/4);
        //rotate(result, 0,,0);
        //rotate(result, 0,0,-3.1415/4);
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

int Converter::processVideo(QString s)
{
    CvCapture * capture = cvCaptureFromAVI(s.toStdString().c_str());
    if(!capture)
    {
        QMessageBox::warning(0, "Error", "cvCaptureFromAVI failed (file not found?)\n");
        return 0;
    }
    int fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
    qDebug() << "* FPS: %d" <<  fps << "\n";
    IplImage* frame = NULL;
    int frame_number = 0;
    while ((frame = cvQueryFrame(capture))) {
        char filename[100];
        strcpy(filename, "frame_");
        char frame_id[30];
        sprintf(frame_id,"%d",frame_number);
        strcat(filename, frame_id);
        strcat(filename, ".bmp");
        QMatrix matrix;
        matrix.rotate(180);
        QImage img = IplImage2QImage(frame).transformed(matrix).mirrored(true, false);
        img.save(QString(filename), "BMP", 100);
        frame_number++;
    }
    cvReleaseCapture(&capture);
    return frame_number;
}

double Converter::calculate(QVector<int> &res, QVector<double> &pres, int val)
{
    /*std::cerr << val << "\n";
    boost::numeric::ublas::matrix<double> A(res.size(),res.size());
    boost::numeric::ublas::vector<double> b(res.size());
    for(int i = 0;i < res.size();++i){
        for(int j = 0 ; j < res.size();++j){
            if(j == res.size()-1){
                A(i,j) = 1.0;
                continue;
            }
            A(i,j) = std::pow(res[i],res.size()-1-j);
        }
        b[i] = pres[i];
    }
    boost::numeric::ublas::vector<double> a = boost::math::tools::solve(A,b);
    std::cerr << A << "\n";
    std::cerr << b << "\n";
    std::cerr << a << "\n";*/
    double sum = 0.0,x1 = 0.0,y1 = 0.0,y0=0.0,x0=0.0;
    /*for(auto it = a.begin(); it != a.end(); ++it){
        sum += (*it)*pow(val,a.size()-1-(int)(it-a.begin()));
    }
    std::cerr << sum << "\n";*/
    for(auto it = res.begin(); it != res.end(); ++it){
        if(*it > val){
            if(it == res.begin()){
                return 0;
            }
            x1 = *it;
            y1 = pres[it - res.begin()];
            x0 = *(it-1);
            y0 = pres[it - res.begin() - 1];
            break;
        }
        if(it + 1 == res.end()){
            return 0;
        }
    }
    sum = y0 + (y1 - y0) * (val - x0) / ( x1 - x0);
    return sum;

}
