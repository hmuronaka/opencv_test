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
Mat canny(const Mat& image, double threshold1, double threshold2, int apertureSize = 3, bool L2gradient = false,
          bool is_show = true);
Mat hough_lines_p(const Mat& image, double rho, double theta, int threshold, double srn = 0, double stn = 0,
                vector<Vec4i>* points = NULL, bool is_show = true);Mat hough_lines(const Mat& image, double rho, double theta, int threshold, double srn = 0, double stn = 0,
                vector<Vec4i>* points = NULL, bool is_show = true);
Mat smooth(const Mat& image, bool is_show = true);
Mat fastNlMeansDenoising(const Mat& image, bool is_show = true);
Mat medianBlur(const Mat& image, bool is_show = true);
Mat delete_noize(const Mat& image, int value = 1, bool is_show = true);
Mat convert_color(const Mat& mat, int val, bool is_show = true);
Mat resize(const Mat& image, Size dsize, double fx, double fy);
Mat good_features_to_track(const Mat& image, int maxCorners = 4, double qualityLevel = 0.01, double minDistance = 100,
                           int blockSize = 3, bool useHarrisDetector = false, double k = 0.04,
                           vector<Point2f>* points = NULL, bool is_show = true);
void show_window(const string& window_name, const Mat& mat, bool is_show = true);
Mat delete_box(const Mat& image, bool is_show = true);
void print_mat(const Mat& r1);

// プライベート系
Mat draw_lines(const Mat& mat, const vector<Vec4i>& lines);
Mat draw_rectangle(const Mat& mat, const vector<Point>& points);
double distance(const Point& pt1, const Point& pt2);
vector<Point> get_points(const vector<Vec4i>& lines);

#endif
