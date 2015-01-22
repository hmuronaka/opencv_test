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
    
    hough_lines(
                 canny(
                       delete_noize(image, 10)
                       , 80, 255, 3, false, is_show)
                , 1, CV_PI / 180, 100, 0, 0, &points, is_show);
    
    Mat result_image = draw_lines(image, points);
    show_window("result", result_image);
    
    return result_image;
}

int main (int argc, char **argv)
{
    char path[FILENAME_MAX];
    char extensions[][16] = {".png", ".jpg"};
    int ext_len = sizeof(extensions) / sizeof(extensions[0]);
    for(int i = 1; i < 99; i++) {
        Mat image;
        int j = 0;
        for(j = 0; j < ext_len; j++) {
            sprintf(path, "%s/sample%03d%s", BASE_PATH.c_str(), i, extensions[j]);
            image = imread(path, 0);
            if( image.data ) {
                cout << "read " << path << "." << endl;
                break;
            }
        }
        
        if( !image.data ) {
            cout << "end.";
            break;
        }
        image = resize(image, Size(), 0.2, 0.2);
        Mat result_image = testcase1(image);
        
        sprintf(path, "%s/sample%03d_out%s", BASE_PATH.c_str(), i, extensions[j]);
        cv::imwrite(path, result_image);
    }
    return 0;
}

