//#include<opencv2/core/core.hpp>
//#include<opencv2/highgui/highgui.hpp>
//#include<opencv2/imgproc/imgproc.hpp>

//#include<iostream>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
 
using namespace cv;
using namespace std;


int main() {

//Generic variables
	char charCheckForEscKey = 0;
	unsigned char key;
	int count = 0;
	
	int lowR = 0;					// Set Hue
	int highR = 255;

	int lowG = 0;					// Set Saturation
	int highG = 255;

	int lowB = 25;					// Set Value
	int highB = 255;
	
	int lowH = 50;					// Set Hue
	int highH = 80;

	int lowS = 120;					// Set Saturation
	int highS = 255;

	int lowV = 0;					// Set Value
	int highV = 255;

	
//Initiate variables for writing the name of file
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
	std::vector<cv::Vec3f> v3fCircles;		// 3 element vector of floats, this will be the pass by reference output of HoughCircles()

	//cvNamedWindow( "Example2", CV_WINDOW_AUTOSIZE );//Open a gui window
	capture = cvCaptureFromCAM(0);//Initiate CvCapture structure for reading a video stream from the camera
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 1280 );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 960 );


	while (charCheckForEscKey != 27 ) {		// until the Esc is pressed or webcam connection is lost

        	frame = cvQueryFrame( capture );//get a single frame in IplImage format
        	if( !frame ){
			std::cout<<"Could not fetch frame"<<endl;
			break;
		} 
		imgOriginal = cv::cvarrToMat(frame);//convert IplImage to cv::mat
		cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);			// Convert Original Image to HSV Thresh Image

		cv::inRange(hsvImg, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), threshImg);

		cv::GaussianBlur(threshImg, threshImg, cv::Size(3, 3), 0);			//Blur Effect
		cv::dilate(threshImg, threshImg, 0);						// Dilate Filter Effect
		cv::erode(threshImg, threshImg, 0);						// Erode Filter Effect
		cv::Canny(threshImg, threshImg, 50, 100, 3);
		// fill circles vector with all circles in processed image
		cv::HoughCircles(threshImg,v3fCircles,CV_HOUGH_GRADIENT,2,threshImg.rows / 4,100,50,0,70);  // algorithm for detecting circles		
		if(v3fCircles.size() == 0){
			std::cout<<"Ball not found"<<endl;
		}
		for (int i = 0; i < v3fCircles.size(); i++) {				// for each circle
															
			std::cout << "Ball position X = "<< v3fCircles[i][0] // x position of center point of circle
				<<",\tY = "<< v3fCircles[i][1]	 	// y position of center point of circle
				<<",\tRadius = "<< v3fCircles[i][2]<< "\n";	// radius of circle

										// draw small green circle at center of object detected
			cv::circle(imgOriginal,						// draw on original image
				cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]), // center point of circle
				3,							// radius of circle in pixels
				cv::Scalar(0, 255, 0),					// draw green
				CV_FILLED);						// thickness

																				// draw red circle around object detected 
			cv::circle(imgOriginal,						// draw on original image
				cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),// center point of circle
				(int)v3fCircles[i][2],					// radius of circle in pixels
				cv::Scalar(0, 0, 255),					// draw red
				3);							// thickness
		}	

		// declare windows
		cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);
		cv::namedWindow("threshImg", CV_WINDOW_NORMAL);	

	    /* Create trackbars in "threshImg" window to adjust according to object and environment.*/
		cv::createTrackbar("LowH", "threshImg", &lowH, 179);	//Hue (0 - 179)
		cv::createTrackbar("HighH", "threshImg", &highH, 179);

		cv::createTrackbar("LowS", "threshImg", &lowS, 255);	//Saturation (0 - 255)
		cv::createTrackbar("HighS", "threshImg", &highS, 255);

		cv::createTrackbar("LowV", "threshImg", &lowV, 255);	//Value (0 - 255)
		cv::createTrackbar("HighV", "threshImg", &highV, 255);
		

		cv::imshow("imgOriginal", imgOriginal);					// show windows
		cv::imshow("threshImg", threshImg);

		charCheckForEscKey = cv::waitKey(1);					// delay and get key press
	}

	std::cout<<"Hue:"<<lowH<<" - "<<highH<<endl;
	std::cout<<"Saturation:"<<lowS<<" - "<<highS<<endl;
	std::cout<<"Value:"<<lowV<<" - "<<highV<<endl;
	
	return(0);											
}

