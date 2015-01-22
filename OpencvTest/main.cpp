//
//  main.cpp
//  OpencvTest
//
//  Created by Muronaka Hiroaki on 2015/01/14.
//  Copyright (c) 2015年 Muronaka Hiroaki. All rights reserved.

#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <opencv2/opencv.hpp>
#include <string>
#include<iostream>
#include<cstring>
using namespace cv;
using namespace std;

void recognize_image(const string& image_path);
Mat threshold(const Mat& image);
Mat erode(const Mat& image, const Point& pt, int val);
Mat sobel(const Mat& image);
Mat laplacian(const Mat& image);
Mat canny(const Mat& image);
Mat hough_lines_p(const Mat& image);
Mat hough_lines(const Mat& image, vector<Vec4i>* points = NULL);
Mat smooth(const Mat& image);
Mat fastNlMeansDenoising(const Mat& image);
Mat medianBlur(const Mat& image);
Mat good_features_to_track(const Mat& image);
Mat match_template(const Mat& image, const Mat& template_image);
Mat delete_noize(const Mat& image);
double distance(const Point& pt1, const Point& pt2);
Mat draw_lines(const Mat& mat, const vector<Vec4i>& lines);
void show_window(const string& window_name, const Mat& mat);
Mat convert_color(const Mat& mat, int val);
vector<Vec4i> judge_points(const Mat& mat, const vector<Vec4i>& lines);

static const string IMAGE_FILEPATH="/Users/h_muronaka/Documents/opencv samples/sample001.jpg";
static const string OUT_FILEPATH="/Users/h_muronaka/Documents/opencv samples/temp.jpg";

static Mat template_image;


int main (int argc, char **argv)
{
//    CvCapture *capture = 0;
//    IplImage *frame = 0;
//    double w = 320, h = 240;
//    int c;
    
//    // (1)コマンド引数によって指定された番号のカメラに対するキャプチャ構造体を作成する
//    if (argc == 1 || (argc == 2 && strlen (argv[1]) == 1 && isdigit (argv[1][0])))
//        capture = cvCreateCameraCapture (argc == 2 ? argv[1][0] - '0' : 0);
//    
//    /* この設定は，利用するカメラに依存する */
//    // (2)キャプチャサイズを設定する．
//    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_WIDTH, w);
//    cvSetCaptureProperty (capture, CV_CAP_PROP_FRAME_HEIGHT, h);
//    
//    cvNamedWindow ("Capture", CV_WINDOW_AUTOSIZE);
    Mat image = imread(IMAGE_FILEPATH.c_str(), 0);
    
//    string template_image_path = "/Users/hmu/Documents/opencv samples/pattern.png";
//    template_image = imread(template_image_path, CV_LOAD_IMAGE_GRAYSCALE);
//    hough_lines_p(
////      medianBlur(
//          laplacian(
//              threshold(image)
//                )
////            )
//              );
    
    vector<Vec4i> points;
    Mat result_image =
        hough_lines(canny(image), &points);
//                           medianBlur(
//                                      laplacian(
//                                                //delete_noize(
//                                                threshold(image))));
    points = judge_points(threshold(image), points);
    
    
    
//    match_template(laplacian(threshold(image)), template_image
//                               );
    cv::imwrite(OUT_FILEPATH, result_image);
    
//    recognize_image(image_filename);
    
//    // (3)カメラから画像をキャプチャする
//    while (1) {
//        frame = cvQueryFrame (capture);
//        cvShowImage ("Capture", frame);
//        c = cvWaitKey (2);
//        if (c == '\x1b')
//            break;
//    }
    
//    cvReleaseCapture (&capture);
//    cvDestroyWindow ("Capture");
//    
    return 0;
}

void show_window(const string& window_name, const Mat& mat) {
    namedWindow(window_name, 0);
    imshow(window_name, mat);
    waitKey(0);
    destroyWindow(window_name);
}

Mat medianBlur(const Mat& image) {
    Mat result;
    medianBlur(image, result, 3);
    show_window("medianBlur", result);
    return result;
}

Mat fastNlMeansDenoising(const Mat& mat) {
    Mat result_image;
    cv::fastNlMeansDenoising(mat, result_image);
    show_window("fastNlMeansDenoising", result_image);
    return result_image;
}

Mat smooth(const Mat& image) {
    Mat result_image;
    cv::blur(image, result_image, cv::Size(10, 10));
    show_window("blur", result_image);
    return result_image;
    
    
}

Mat draw_lines(const Mat& mat, const vector<Vec4i>& lines) {
//    Mat result_image = Mat::zeros(mat.rows, mat.cols, mat.type());
    Mat result_image = mat;
//    string image_path = "/Users/hmu/Documents/opencv samples/sample005.jpg";
//    Mat image = imread(image_path.c_str(), 0);
//    result_image = image;
//    cvtColor(result_image, result_image, CV_GRAY2BGR);
    
    for(size_t i = 0; i < lines.size(); i++) {
        cv::line(result_image, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(255, 128, 0), 1, 8);
    }
    return result_image;
}

Mat draw_rectangle(const Mat& mat, const vector<Point>& points) {
    Mat result_image = Mat::zeros(mat.rows, mat.cols, mat.type());
    cvtColor(result_image, result_image, CV_GRAY2BGR);
    
    line(result_image, points[0], points[1], Scalar(255, 128, 0), 3, 8);
    line(result_image, points[0], points[2], Scalar(255, 128, 0), 3, 8);
    line(result_image, points[1], points[3], Scalar(255, 128, 0), 3, 8);
    line(result_image, points[2], points[3], Scalar(255, 128, 0), 3, 8);
    
    return result_image;
}

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
    
    FILE* fp = fopen("/Users/h_muronaka/result.txt", "w");
    for(int i = 0; i < lines.size(); i++) {
//        if(lines[i][0] < leftTop.x && lines[i][1] < leftTop.y) {
//            leftTop.x = lines[i][0];
//            leftTop.y = lines[i][1];
//        }
//        if( lines[i][2] > rightTop.x && lines[i][3] < rightTop.y ) {
//            rightTop.x = lines[i][2];
//            rightTop.y = lines[i][3];
//        }
//        if( lines[i][2] < leftBottom.x && lines[i][3] > leftBottom.y ) {
//            leftBottom.x = lines[i][2];
//            leftBottom.y = lines[i][3];
//        }
//        if( lines[i][2] > rightBottom.x && lines[i][3] > rightBottom.y ) {
//            rightBottom.x = lines[i][2];
//            rightBottom.y = lines[i][3];
//        }
        
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
        fprintf(fp, "%d, %d, %d, %d, %d\n",
                i, lines[i][0], lines[i][1], lines[i][2], lines[i][3]);
        
    }
    fclose(fp);
    vector<Point> result;
    result.push_back(leftTop);
    result.push_back(rightTop);
    result.push_back(leftBottom);
    result.push_back(rightBottom);
    return result;
}

Mat threshold(const Mat& image) {
    Mat result_image;
// 白紙OK
//    threshold(image, result_image, 128,255, CV_THRESH_BINARY);
    threshold(image, result_image, 170,255, CV_THRESH_BINARY);
    show_window("threshold", result_image);
    return result_image;
}

Mat hough_lines(const Mat& image, vector<Vec4i>* points) {
    vector<Vec2f> lines;
//    HoughLines(image, lines, 1, CV_PI/180, 100, 100, 5 );
    HoughLines(image, lines, 1, CV_PI/180, 150, 0, 5);
    
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
//    Mat in_image = imread(IMAGE_FILEPATH.c_str());
//    Mat result_image = in_image;
    Mat result_image = Mat::zeros(image.rows, image.cols, image.type());
    result_image = draw_lines(result_image, result_lines);
    show_window("hough_line", result_image);
    if( points != NULL ) {
        *points = result_lines;
    }
    return result_image;
}


Mat hough_lines_p(const Mat& image) {
    vector<Vec4i> result_lines;
    
    cv::HoughLinesP(image, result_lines, 1, CV_PI / 180 * 2, 100, 100, 5);
    
    Mat result_image = draw_lines(image, result_lines);
    show_window("hough_lines_p", result_image);
    
    vector<Point> points = get_points(result_lines);
    result_image = draw_rectangle(image, points);
    show_window("hough_lines_p_rectangle", result_image);
    
    
    return result_image;
}

Mat erode(const Mat& image, const Point& pt, int val) {
    Mat result_image;
    erode(image, result_image, cv::Mat(), pt, val);
    show_window("erode", result_image);
    return result_image;
}

//Mat threshold(const Mat& image) {
//    Mat src_image = image;
//    Mat result_image;
//    
//    for(double thres = 1; thres <=255; thres += 10) {
//        threshold(src_image, result_image, thres, 255, THRESH_TOZERO | CV_THRESH_OTSU);
//        show_window("threshold", result_image);
//    }
//    return result_image;
//}

Mat sobel(const Mat& image) {
    Mat result_image;
    Sobel(image, result_image, CV_32F, 1, 1);
    convertScaleAbs(result_image, result_image, 1, 0);
    show_window("sobel", result_image);
    return result_image;
}

Mat laplacian(const Mat& image) {
    Mat result_image;
    Laplacian(image, result_image, CV_32F, 1, 1);
    convertScaleAbs(result_image, result_image, 1, 0);
    show_window("sobel", result_image);
    return result_image;
}

Mat canny(const Mat& image) {
    Mat result_image;
    Canny(image, result_image, 224, 255, 3);
    show_window("canny", result_image);
    return result_image;
}


void recognize_image(const string& image_path) {
    Mat image = imread(image_path.c_str(), 0);
    if( !image.data ) {
        printf("imread error");
        return;
    }
    
    Mat after_image;
    for(int depth = 1; depth <= 31; depth+=2) {
//        Sobel(image, after_image, CV_32F, depth, depth, depth);
        Laplacian(image, after_image, CV_32F, 1, 1, depth);
        //Canny(image, after_image, 50, 200);
        Mat result_image;
        convertScaleAbs(after_image, result_image, 1, 0);
        
        namedWindow("Display Image", WINDOW_AUTOSIZE);
        imshow("Display Image", result_image);
        waitKey(0);
        cvDestroyWindow("Display Image");
    }
}


Mat good_features_to_track(const Mat& image) {
    Mat result = image;
    vector<Point2f> corners;
    goodFeaturesToTrack(image, corners, 4, 0.01, 100, Mat(), 5, false);
    vector<Point2f>::iterator ite = corners.begin();
    string image_path = IMAGE_FILEPATH;
    Mat temp  = imread(image_path.c_str(), CV_32F);
    while( ite != corners.end() ) {
        circle(temp, Point(ite->x, ite->y), 4, Scalar(0, 0, 255), -1);
        circle(temp, Point(ite->x, ite->y), 8, Scalar(0, 0, 255));
        ++ite;
    }
    show_window("good_features_to_track", temp);
    return result;
}

Mat match_template(const Mat& image, const Mat& template_image) {
    Mat result_image(image.rows - template_image.rows + 1, image.cols - template_image.cols + 1, CV_8U);
    //template_image.convertTo(template_image, image.type());
    
    matchTemplate(image, template_image, result_image, CV_TM_CCOEFF_NORMED);
//    normalize(result_image, result_image, 0, 1, NORM_MINMAX, -1, Mat());
    double minVal, maxVal;
    Point matchLoc, minLoc, maxLoc;
    minMaxLoc(result_image, &minVal, &maxVal, &minLoc, &maxLoc, Mat());
    
    Mat img = image;
    rectangle(img, maxLoc, Point(maxLoc.x + template_image.cols, maxLoc.y + template_image.rows), Scalar(255,0, 0), 2, 8, 0);
//    rectangle(result_image, maxLoc, Point(maxLoc.x + template_image.cols, maxLoc.y + template_image.rows), Scalar::all(0), 2, 8, 0);
    show_window("match_template", img);
    return result_image;
}

Mat delete_noize(const Mat& image) {
    Mat result_image;
    int value = 3;
//    value = 100;
    cv::erode(image, result_image, Mat(), Point(-1, -1), value);
    cv::dilate(result_image, result_image, Mat(), Point(-1, -1), value);
    show_window("delete_noize", result_image);
    return result_image;
}

double distance(const Point& pt1, const Point& pt2) {
    return sqrt(pow(pt2.x - pt1.x, 2) + pow(pt2.y - pt1.y, 2));
}

Mat convert_color(const Mat& mat, int value) {
    Mat result_image;
    cvtColor(mat, result_image, value);
    return result_image;
}


vector<Vec4i> judge_points(const Mat& mat, const vector<Vec4i>& lines) {
    Mat
}