#ifndef VISION_H
#define VISION_H

#include <iostream>
#include "GPIOClass.h"
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include "GPIOClass.h"
#include "GPIOFun.h"
#include "GOPIFun.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
using namespace std;
// Bot vision functions
class vision
{
private:
	char *fileno_char;
	std::string ipl="ipl";
	std::string mat="mat";
	std::string head="test";
	std::string gray="gray";
	std::string hsv="hsv";
	std::string thresh="thresh";
	std::string post="post";
	std::string blue="blue";	
	std::string green="green";
	std::string red="red";
	std::string ext=".jpg";
	std::string filename;
	std::stringstream fileno;
	std::string fileno_str;   
	char *filenamechar = new char[30];	
//Initiate varibles to fetch frames from webcam
	IplImage* frame = 0;    	
	CvCapture* capture = 0;
	cv::Mat imgOriginal;		// Input image
	cv::Mat hsvImg;			// HSV Image
	cv::Mat threshImg;		// Thresh Image
	cv::Mat grayImg;		//Gray image
	cv::Mat BGRImg[3];		//seperate blue, green and red channels
        cv::Mat blue_thresh;		//Thresholded image for blue
        cv::Mat green_thresh;		//Thresholded image for green
        cv::Mat red_thresh;		//Thresholded image for red
	std::vector<cv::Vec3f> v3fCircles;// 3 element vector of floats, this will be the pass by reference output of HoughCircles()
	const long int second         = 100000;
	double* set_env(void);
	double* set_env_rgb(void);
	int find_balls();

public:
	double* pointer_env;
	double env[6];
	double x_cord;
	double y_cord;
	double radius;
	
	vision();
	void clean();
	int locate_ball(GOPI pi);
};

int find_balls(double* point_env,double* x_cord, double* y_cord, double* radius);
int locate_ball(double* point_env,double *x,double *y, double *r);
int locate_ball(double* point_env,double *x,double *y, double *r,GOPI pi);

#endif
