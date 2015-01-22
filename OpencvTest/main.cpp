//
//  main.cpp
//  OpencvTest
//
//  Created by Muronaka Hiroaki on 2015/01/14.
//  Copyright (c) 2015年 Muronaka Hiroaki. All rights reserved.


#include<string>
#include<cstdlib>
#include<iostream>
#include<cstring>
#include"opencv_util.h"
using namespace std;


static const string BASE_PATH="/Users/hmu/src/opencv_test/samples";

Mat testcase1(const Mat& image) {
    bool is_show = false;
    vector<Vec4i> points;
    
    hough_lines(canny(image, is_show), &points, is_show);
    
    Mat result_image = draw_lines(image, points);
    show_window("result", result_image);
    
    return result_image;
}

int main (int argc, char **argv)
{
    char path[FILENAME_MAX];
    for(int i = 1; i < 99; i++) {
        sprintf(path, "%s/sample%03d.png", BASE_PATH.c_str(), i);
        
        Mat image = imread(path, 0);
        if( image.data ) {
            cout << "read " << path << "." << endl;
        } else {
            cout << "end." << endl;
            break;
        }
        Mat result_image = testcase1(image);
        
        sprintf(path, "%s/sample%03d_out.png", BASE_PATH.c_str(), i);
        cv::imwrite(path, result_image);
    }
    return 0;
}

