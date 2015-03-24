#include "detector.hpp"
#include "imageconverter.hpp"
using namespace cv;

Detector::Detector(QObject *parent) : QObject(parent)
{
    
}

QRect Detector::detect(QImage templ, QImage image)
{
    Mat t = ImageConverter::QImage2Mat(templ);
    Mat m = ImageConverter::QImage2Mat(image);
    Point p = MatchingMethod(m, t, 5);
    QRect res(p.x, p.y, templ.width(), templ.height());
    return res;
}

Rect Detector::detect(Mat t, Mat m)
{
    Point p = MatchingMethod(m, t, 5);
    Rect r;
    r.x = p.x;
    r.y = p.y;
    r.width = t.cols;
    r.height = t.rows;
    return r;
}

Detector::~Detector()
{
    
}

Point Detector::MatchingMethod(Mat img, Mat templ, int match_method)
{
    /// Source image to display
    Mat img_display, result;
    img.copyTo(img_display);
    
    /// Create the result matrix
    int result_cols = img.cols - templ.cols + 1;
    int result_rows = img.rows - templ.rows + 1;
    
    result.create(result_cols, result_rows, CV_32FC1);
    
    /// Do the Matching and Normalize
    matchTemplate(img, templ, result, match_method);
    normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());
    
    /// Localizing the best match with minMaxLoc
    double minVal;
    double maxVal;
    Point minLoc;
    Point maxLoc;
    Point matchLoc;
    matchLoc.x = -1;
    matchLoc.y = -1;
    
    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    
    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all
    /// the other methods, the higher the better
    if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED) {
        matchLoc = minLoc;
    } else {
        matchLoc = maxLoc;
    }
    
    /// Show me what you got
//    rectangle(img_display, matchLoc,
//              Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows),
//              Scalar::all(255), 2, 8, 0);
    // rectangle( result, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y +
    // templ.rows ), Scalar::all(255), 2, 8, 0 );
    
    //imshow("Im", img_display);
    //cv::waitKey();
    // imshow( result_window, result );
    
    return matchLoc;
}
