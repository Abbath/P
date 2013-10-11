#include "converter.hpp"

Converter::Converter()
{
}

void Converter::rotate(QVector<Line> &l, double angle_x, double angle_y, double angle_z){
    Point origin = {0,0,0};
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
