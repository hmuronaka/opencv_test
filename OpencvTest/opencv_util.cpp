//
//  opencv_util.cpp
//  OpencvTest
//
//  Created by Muronaka Hiroaki on 2015/01/22.
//  Copyright (c) 2015年 Muronaka Hiroaki. All rights reserved.
//
#include "opencv_util.h"

void show_window(const string& window_name, const Mat& mat, bool is_show) {
    if( is_show ) {
        namedWindow(window_name, 0);
        imshow(window_name, mat);
        waitKey(0);
        destroyWindow(window_name);
    }
}

Mat medianBlur(const Mat& image, bool is_show) {
    Mat result;
    medianBlur(image, result, 3);
    show_window("medianBlur", result, is_show);
    return result;
}

Mat fastNlMeansDenoising(const Mat& mat, bool is_show) {
    Mat result_image;
    cv::fastNlMeansDenoising(mat, result_image);
    show_window("fastNlMeansDenoising", result_image, is_show);
    return result_image;
}

Mat smooth(const Mat& image, bool is_show) {
    Mat result_image;
    cv::blur(image, result_image, cv::Size(10, 10));
    show_window("smooth", result_image, is_show);
    return result_image;
}

Mat threshold(const Mat& image, bool is_show) {
    Mat result_image;
    threshold(image, result_image, 170,255, CV_THRESH_BINARY);
    show_window("threshold", result_image, is_show);
    return result_image;
}

Mat hough_lines(const Mat& image, vector<Vec4i>* points, bool is_show) {
    vector<Vec2f> lines;
    HoughLines(image, lines, 5, CV_PI/180, 200, 10, 5);
    
    vector<Vec4i> result_lines;
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        result_lines.push_back(Vec4i(pt1.x, pt1.y, pt2.x, pt2.y));
    }
    
    Mat result_image = Mat::zeros(image.rows, image.cols, image.type());
    result_image = draw_lines(result_image, result_lines);
    show_window("hough_line", result_image, is_show);
    if( points != NULL ) {
        *points = result_lines;
    }
    return result_image;
}


Mat hough_lines_p(const Mat& image, bool is_show) {
    vector<Vec4i> result_lines;
    
    cv::HoughLinesP(image, result_lines, 1, CV_PI / 180 * 2, 100, 100, 5);
    
    Mat result_image = Mat::zeros(image.rows, image.cols, image.type());
    result_image = draw_lines(result_image, result_lines);
    show_window("hough_lines_p", result_image, is_show);
    
    vector<Point> points = get_points(result_lines);
    result_image = draw_rectangle(image, points);
    show_window("hough_lines_p_rectangle", result_image, is_show);
    
    return result_image;
}

Mat erode(const Mat& image, const Point& pt, int val, bool is_show) {
    Mat result_image;
    erode(image, result_image, cv::Mat(), pt, val);
    show_window("erode", result_image, is_show);
    return result_image;
}

Mat sobel(const Mat& image, bool is_show) {
    Mat result_image;
    Sobel(image, result_image, CV_32F, 1, 1);
    convertScaleAbs(result_image, result_image, 1, 0);
    show_window("sobel", result_image, is_show);
    return result_image;
}

Mat laplacian(const Mat& image, bool is_show) {
    Mat result_image;
    Laplacian(image, result_image, CV_32F, 1, 1);
    convertScaleAbs(result_image, result_image, 1, 0);
    show_window("sobel", result_image, is_show);
    return result_image;
}

Mat canny(const Mat& image, bool is_show) {
    Mat result_image;
    Canny(image, result_image, 224, 255, 3);
    show_window("canny", result_image, is_show);
    return result_image;
}


Mat good_features_to_track(const Mat& image, vector<Point2f>* points, bool is_show) {
    Mat result = image;
    vector<Point2f> corners;
    goodFeaturesToTrack(image, corners, 4, 0.01, 100, Mat(), 5, false);
    vector<Point2f>::iterator ite = corners.begin();
    while( ite != corners.end() ) {
        circle(result, Point(ite->x, ite->y), 4, Scalar(0, 0, 255), -1);
        circle(result, Point(ite->x, ite->y), 8, Scalar(0, 0, 255));
        ++ite;
    }
    show_window("good_features_to_track", result, is_show);
    if( points != NULL ) {
        *points = corners;
    }
    return result;
}

Mat delete_noize(const Mat& image, int value, bool is_show) {
    Mat result_image;
    cv::erode(image, result_image, Mat(), Point(-1, -1), value);
    cv::dilate(result_image, result_image, Mat(), Point(-1, -1), value);
    show_window("delete_noize", result_image, is_show);
    return result_image;
}

double distance(const Point& pt1, const Point& pt2) {
    return sqrt(pow(pt2.x - pt1.x, 2) + pow(pt2.y - pt1.y, 2));
}

Mat convert_color(const Mat& mat, int value, bool is_show) {
    Mat result_image;
    cvtColor(mat, result_image, value);
    show_window("convert_color", result_image, is_show);
    return result_image;
}

Mat draw_lines(const Mat& mat, const vector<Vec4i>& lines) {
    Mat result_image = mat;
    cvtColor(result_image, result_image, CV_GRAY2BGR);
    
    for(size_t i = 0; i < lines.size(); i++) {
        cv::line(result_image, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(255, 128, 0), 1, 8);
    }
    return result_image;
}

// matに対して、四角形を引いた結果を返す。
// pointsの順番は,
// left_top
// right_top
// left_bottom
// right_bottom
Mat draw_rectangle(const Mat& mat, const vector<Point>& points) {
    Mat result_image = mat;
    cvtColor(result_image, result_image, CV_GRAY2BGR);
    
    Scalar scalar = Scalar(255,128, 0);
    int thickness = 3;
    int line_type = 8;
    
    line(result_image, points[0], points[1], scalar, thickness, line_type);
    line(result_image, points[0], points[2], scalar, thickness, line_type);
    line(result_image, points[1], points[3], scalar, thickness, line_type);
    line(result_image, points[2], points[3], scalar, thickness, line_type);
    
    return result_image;
}

// 線の始点/終点から、一番外側の線(left_top, right_top, left_bottom, right_bottom)を取得する
vector<Point> get_points(const vector<Vec4i>& lines) {
    Point leftTop(-1, -1);
    Point rightTop(-1, -1);
    Point leftBottom(-1, -1);
    Point rightBottom(-1, -1);
    
    if( lines.size() > 0 ) {
        leftTop.x = lines[0][0];
        leftTop.y = lines[0][1];
        rightTop = leftBottom = rightBottom = leftTop;
    }
    
    for(int i = 0; i < lines.size(); i++) {
        if(lines[i][0] < leftTop.x) {
            leftBottom.x = leftTop.x = lines[i][0];
        }
        if( lines[i][1] < leftTop.y) {
            rightTop.y = leftTop.y = lines[i][1];
        }
        if( lines[i][2] > rightTop.x ) {
            rightBottom.x = rightTop.x = lines[i][2];
        }
        if( lines[i][3] > rightBottom.y ) {
            leftBottom.y = rightBottom.y = lines[i][3];
        }
    }
    
    vector<Point> result;
    result.push_back(leftTop);
    result.push_back(rightTop);
    result.push_back(leftBottom);
    result.push_back(rightBottom);
    return result;
}