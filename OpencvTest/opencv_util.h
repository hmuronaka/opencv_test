//
//  opencv_util.h
//  OpencvTest
//
//  Created by Muronaka Hiroaki on 2015/01/22.
//  Copyright (c) 2015年 Muronaka Hiroaki. All rights reserved.
//

#ifndef OpencvTest_opencv_util_h
#define OpencvTest_opencv_util_h

#include <opencv2/opencv.hpp>
using namespace cv;

// エフェクト系
Mat threshold(const Mat& image, bool is_show = true);
Mat erode(const Mat& image, const Point& pt, int val, bool is_show = true);
Mat sobel(const Mat& image, bool is_show = true);
Mat laplacian(const Mat& image, bool is_show = true);
Mat canny(const Mat& image, bool is_show = true);
Mat hough_lines_p(const Mat& image, bool is_show = true);
Mat hough_lines(const Mat& image, vector<Vec4i>* points = NULL, bool is_show = true);
Mat smooth(const Mat& image, bool is_show = true);
Mat fastNlMeansDenoising(const Mat& image, bool is_show = true);
Mat medianBlur(const Mat& image, bool is_show = true);
Mat good_features_to_track(const Mat& image, vector<Point2f>* points = NULL, bool is_show = true);
Mat delete_noize(const Mat& image, int value = 1, bool is_show = true);
Mat convert_color(const Mat& mat, int val, bool is_show = true);


void show_window(const string& window_name, const Mat& mat, bool is_show = true);

// プライベート系
Mat draw_lines(const Mat& mat, const vector<Vec4i>& lines);
Mat draw_rectangle(const Mat& mat, const vector<Point>& points);
double distance(const Point& pt1, const Point& pt2);
vector<Point> get_points(const vector<Vec4i>& lines);

#endif
