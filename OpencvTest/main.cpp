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

static const double rate = 0.2;

struct Score {
    double score;
    double max_distance;
    
    Score() { score = 0; max_distance = 0; }
    void print() {
        cout << score << ", " << max_distance << endl;
    }
};


static const string BASE_PATH="/Users/hmu/src/opencv_test/samples";

Score get_score(const vector<Point>& answer, const vector<Point>& teacher) {
    
    Score score;
    for(int i = 0; i < answer.size(); i++) {
        double distance_val = distance(answer[i], Point(teacher[i].x * rate, teacher[i].y * rate));
        score.score += distance_val;
        if( score.max_distance < distance_val ) {
            score.max_distance = distance_val;
        }
    }
    return score;
}

void process_hough_lines_p(const Mat& image, vector<Point>& rectangle, Mat* result_image) {
    bool is_show = false;
    vector<Vec4i> points;
    
    hough_lines_p(
                canny(
                      delete_noize(image, 10, is_show)
                     
                      , 80, 255, 3, false, is_show)
                , 1, CV_PI / 180, 100, 0, 0, &points, is_show);
    
    rectangle = get_points(points);
    if( result_image != NULL ) {
        *result_image = draw_rectangle(image, rectangle);
        
        //    Mat result_image = draw_lines(image, points);
        show_window("result", *result_image);
    }
}

Score testcase1(const Mat& image, const vector<Point>& answer, Mat* result_image) {
    
    vector<Point> rectangle;
    process_hough_lines_p(image, rectangle, result_image);
    
    Score score = get_score(rectangle, answer);
    return score;
}

Mat testcase2(const Mat& image) {
    bool is_show = false;
    vector<Vec4i> points;
    
    Mat result_image = good_features_to_track(
        hough_lines(
                canny(
                      delete_noize(image, 10, is_show)
                      , 80, 255, 3, false, is_show)
                    , 1, CV_PI / 180, 100, 0, 0, &points, is_show), 8, 0.05, 10, 2);
    
    show_window("result", result_image);
    
    return result_image;
}

vector<Point> get_teacher(const string& filename) {
    FILE* fp = fopen(filename.c_str(), "r");
    if( fp == NULL ) {
        return vector<Point>();
    }
    
    vector<Point> teacher(4);
    for(int i = 0; i < teacher.size(); i++) {
        int x;
        int y;
        fscanf(fp, "%d,%d", &x, &y);
        teacher[i] = Point(x, y);
    }
    fclose(fp);
    return teacher;
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
            continue;
        }
        
        sprintf(path, "%s/sample%03d.csv", BASE_PATH.c_str(), i);
        vector<Point> teacher = get_teacher(path);
        
        image = resize(image, Size(), rate, rate);
        Mat result_image;
        Score score = testcase1(image, teacher, &result_image);
        score.print();
        
        sprintf(path, "%s/sample%03d_out%s", BASE_PATH.c_str(), i, extensions[j]);
        cv::imwrite(path, result_image);
    }
    return 0;
}

