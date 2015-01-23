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

Mat hough_lines(const Mat& image, double rho, double theta, int threshold, double srn, double stn, vector<Vec4i>* points, bool is_show) {
    vector<Vec2f> lines;
    HoughLines(image, lines, rho, theta, threshold, srn, stn);
    
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


Mat hough_lines_p(const Mat& image, double rho, double theta, int threshold, double srn, double stn, vector<Vec4i>* points, bool is_show) {
    vector<Vec4i> result_lines;
    
    cv::HoughLinesP(image, result_lines, rho, theta, threshold, srn, stn);
    
    Mat result_image = Mat::zeros(image.rows, image.cols, image.type());
    result_image = draw_lines(result_image, result_lines);
    show_window("hough_lines_p", result_image, is_show);
    
    if( points != NULL ) {
        *points = result_lines;
    }
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

Mat canny(const Mat& image, double threshold1, double threshold2, int apertureSize, bool L2gradient, bool is_show) {
    Mat result_image;
    Canny(image, result_image, threshold1, threshold2, apertureSize, L2gradient);
    show_window("canny", result_image, is_show);
    return result_image;
}


Mat good_features_to_track(const Mat& image, int maxCorners, double qualityLevel, double minDistance, int blockSize,
                           bool useHarrisDetector, double k, vector<Point2f>* points, bool is_show) {
    Mat result = image;
    vector<Point2f> corners;
    goodFeaturesToTrack(image, corners, maxCorners, qualityLevel, minDistance, Mat(), blockSize, useHarrisDetector, k);
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

Mat resize(const Mat& image, Size dsize, double fx, double fy) {
    Mat result_image;
    cv::resize(image, result_image, dsize, fx, fy);
    return result_image;
}

Mat draw_lines(const Mat& mat, const vector<Vec4i>& lines) {
    Mat result_image = mat;
//    cvtColor(result_image, result_image, CV_GRAY2BGR);
    
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

void print_mat(const Mat& r1) {
    // 行数
    std::cout << "rows:" << r1.rows <<std::endl;
    // 列数
    std::cout << "cols:" << r1.cols << std::endl;
    // 次元数
    std::cout << "dims:" << r1.dims << std::endl;
    // サイズ（2次元の場合）
    std::cout << "size[]:" << r1.size().width << "," << r1.size().height << std::endl;
    // ビット深度ID
    std::cout << "depth (ID):" << r1.depth() << "(=" << CV_32F << ")" << std::endl;
    // チャンネル数
    std::cout << "channels:" << r1.channels() << std::endl;
    // （複数チャンネルから成る）1要素のサイズ [バイト単位]
    std::cout << "elemSize:" << r1.elemSize() << "[byte]" << std::endl;
    // 1要素内の1チャンネル分のサイズ [バイト単位]
    std::cout << "elemSize1 (elemSize/channels):" << r1.elemSize1() << "[byte]" << std::endl;
    // 要素の総数
    std::cout << "total:" << r1.total() << std::endl;
    // ステップ数 [バイト単位]
    std::cout << "step:" << r1.step << "[byte]" << std::endl;
    // 1ステップ内のチャンネル総数
    std::cout << "step1 (step/elemSize1):" << r1.step1()  << std::endl;
    // データは連続か？
    std::cout << "isContinuous:" << (r1.isContinuous()?"true":"false") << std::endl;
    // 部分行列か？
    std::cout << "isSubmatrix:" << (r1.isSubmatrix()?"true":"false") << std::endl;
    // データは空か？
    std::cout << "empty:" << (r1.empty()?"true":"false") << std::endl;
}

Mat delete_box(const Mat& image, bool is_show) {
    Mat result_image = image;
    
    const int white_color = 170;
    const int box_color = 90;
    const int black_color = 60;
    
    int count = 0;
    int prev_color = -1;
    int fill_color = -1;
    int state = 0; // 0:init 1:in_paper 2:box 3: outer
    for(int row_idx = 0; row_idx < result_image.rows; row_idx++) {
        count = 0;
        state = 0;
        for(int col_idx = 0; col_idx < result_image.cols; col_idx++) {
            int pos = row_idx * result_image.cols + col_idx;
            int color = result_image.data[pos];
            if( prev_color == -1 ) {
                prev_color = color;
            }
            
            int temp_color = color;
            if( abs(prev_color - color) < 50 ) {
                if( fill_color == -1 ) {
                    fill_color = prev_color;
                }
                color = prev_color;
            } else {
                fill_color = -1;
            }
            prev_color = temp_color;
//            if( state == 0 ) {
//                if( color > white_color ) {
//                    count++;
//                    if( count > 5 ) {
//                        count = 0;
//                        state = 1;
//                    }
//                    color = 255;
//                }
//            } else if( state == 1 ) {
//                if( color < black_color ) {
//                    count++;
//                    if( count > 3 ) {
//                        count = 0;
//                        state = 3;
//                    }
//                    color = 0;
//                } else if( color < box_color ) {
//                    count++;
//                    if( count > 5 ) {
//                        count = 0;
//                        state = 2;
//                    }
//                    color = 255;
//                } else {
//                    color = 255;
//                }
//            } else if( state == 2 ) {
//                if( color > white_color ) {
//                    count++;
//                    if( count > 10 ) {
//                        count = 0;
//                        state = 1;
//                    }
//                    color = 255;
//                } else if( color < black_color ) {
//                    count++;
//                    if( count > 10 ) {
//                        count = 0;
//                        state = 3;
//                    }
//                } else {
//                    color = 255;
//                }
//            } else {
//                if( color > white_color) {
//                    count++;
//                    if( count > 5 ) {
//                        state = 1;
//                        count = 0;
//                    }
//                    color = 255;
//                } else {
//                    color = 0;
//                }
//            }
            result_image.data[pos] = color;
        }
    }
    show_window("delete_box", result_image, is_show);
    return result_image;
}