#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <QObject>
#include <iostream>
#include <vector>
#include <fstream>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


class Detector : public QObject
{
    Q_OBJECT
    cv::Point MatchingMethod(cv::Mat img, cv::Mat templ, int match_method);
public:
    explicit Detector(QObject *parent = 0);
    QRect detect(QImage templ, QImage image);
    cv::Rect detect(cv::Mat t, cv::Mat m);
    ~Detector();
    
signals:
    
public slots:
};

#endif // DETECTOR_HPP
