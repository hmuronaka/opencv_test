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

static const string IMAGE_FILEPATH="/Users/hmu/src/opencv_test/samples/sample003.png";
static const string OUT_FILEPATH="/Users/hmu/src/opencv_test/samples/out.png";

void testcase1(const Mat& image) {
    Mat result_image =
        hough_lines(canny(image));
    
    cv::imwrite(OUT_FILEPATH.c_str(), result_image);
}

int main (int argc, char **argv)
{
    Mat image = imread(IMAGE_FILEPATH.c_str(), 0);
    testcase1(image);
    return 0;
}

