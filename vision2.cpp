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
#include "GPIOClass.h"
#include "GPIOFun.h"
#include "GOPIFun.h"
#include "vision.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
 
using namespace cv;
using namespace std;

vision::vision(){
	int debug_mode;
	const int yes = 1;
	const int no = 0;
	cout<<"Run on debug mode?(yes =1 , no = 0)"<<"\n";
	cin>>debug_mode;
	
	if(debug_mode == yes){
		capture = cvCaptureFromCAM(0);//Initiate CvCapture structure for reading a video stream from the camera
		cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
		cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );
		pointer_env = set_env();
		cvReleaseCapture( &capture );
		/*std::cout<<"Hue:"<<env[0]<<" - "<<env[1]<<endl;
		std::cout<<"Saturation:"<<env[2]<<" - "<<env[3]<<endl;
		std::cout<<"Value:"<<env[4]<<" - "<<env[5]<<endl;*/
	}
	else{
		env[0]= 35.0;
		env[1]= 100.0;
		env[2]= 125.0;
		env[3]= 255.0;
		env[4]= 33.0;
		env[5]= 163.0;
	        pointer_env = env;	
	}


}
void vision::clean(){
	//cvReleaseCapture( &capture );
}
double* vision::set_env() {
//Output array of containing the HSV space values
	static double env[6];

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
	
	int lowH = 35;					// Set Hue
	int highH = 100;

	int lowS = 125;					// Set Saturation
	int highS = 255;

	int lowV = 33;					// Set Value
	int highV = 163;

	//int lowH = 0;					// Set Hue
	//int highH = 8;

	//int lowS = 210;					// Set Saturation
	//int highS = 255;

	//int lowV = 51;					// Set Value
	//int highV = 105;
	
	while (charCheckForEscKey != 27 ) {		// until the Esc is pressed or webcam connection is lost

        	frame = cvQueryFrame( capture );//get a single frame in IplImage format
        	if( !frame ){
			std::cout<<"Could not fetch frame"<<endl;
			break;
		} 
		imgOriginal = cv::cvarrToMat(frame);//convert IplImage to cv::mat
		cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);			// Convert Original Image to HSV Thresh Image

		cv::inRange(hsvImg, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), threshImg);

		cv::GaussianBlur(threshImg, threshImg, cv::Size(9, 9), 0);			//Blur Effect
		cv::dilate(threshImg, threshImg, 0);						// Dilate Filter Effect
		cv::erode(threshImg, threshImg, 0);						// Erode Filter Effect
		//cv::Canny(threshImg, threshImg, 50, 100, 3);
		// fill circles vector with all circles in processed image
		cv::HoughCircles(threshImg,v3fCircles,CV_HOUGH_GRADIENT,2,threshImg.rows / 3,150,40,0,300);  // algorithm for detecting circles		
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
	cvReleaseCapture( &capture );
	cvDestroyWindow( "imgOriginal" );
	cvDestroyWindow( "threshImg" ); 

	env[0] = lowH;
	env[1] = highH;
	
	env[2] = lowS;
	env[3] = highS;

	env[4] = lowV;
	env[5] = highV;

	/*std::cout<<"set_env"<<endl;
	std::cout<<"Hue:"<<env[0]<<" - "<<env[1]<<endl;
	std::cout<<"Saturation:"<<env[2]<<" - "<<env[3]<<endl;
	std::cout<<"Value:"<<env[4]<<" - "<<env[5]<<endl;*/

	return env;											
}
double* vision::set_env_rgb() {
//Output array of containing the HSV space values
	static double env[6];

//Generic variables
	char charCheckForEscKey = 0;
	unsigned char key;
	int count = 0;
	
	int lowR = 0;					// Set Hue
	int highR = 255;

	int lowG = 0;					// Set Saturation
	int highG = 255;


	int lowB = 0;					// Set Saturation
	int highB = 255;
	
	while (charCheckForEscKey != 27 ) {		// until the Esc is pressed or webcam connection is lost

        	frame = cvQueryFrame( capture );//get a single frame in IplImage format
        	if( !frame ){
			std::cout<<"Could not fetch frame"<<endl;
			break;
		} 
		imgOriginal = cv::cvarrToMat(frame);//convert IplImage to cv::mat
		//cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);			// Convert Original Image to HSV Thresh Image

		cv::inRange(imgOriginal, cv::Scalar(lowB, lowG, lowR), cv::Scalar(highB, highG, highR), threshImg);

		cv::GaussianBlur(threshImg, threshImg, cv::Size(9, 9), 0);			//Blur Effect
		cv::dilate(threshImg, threshImg, 0);						// Dilate Filter Effect
		cv::erode(threshImg, threshImg, 0);						// Erode Filter Effect
		//cv::Canny(threshImg, threshImg, 50, 100, 3);
		// fill circles vector with all circles in processed image
		cv::HoughCircles(threshImg,v3fCircles,CV_HOUGH_GRADIENT,2,threshImg.rows / 3,150,40,0,300);  // algorithm for detecting circles		
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
		cv::createTrackbar("LowR", "threshImg", &lowR, 179);	//Hue (0 - 179)
		cv::createTrackbar("HighR", "threshImg", &highR, 179);

		cv::createTrackbar("LowG", "threshImg", &lowG, 255);	//Saturation (0 - 255)
		cv::createTrackbar("HighG", "threshImg", &highG, 255);

		cv::createTrackbar("LowB", "threshImg", &lowB, 255);	//Value (0 - 255)
		cv::createTrackbar("HighB", "threshImg", &highB, 255);
		

		cv::imshow("imgOriginal", imgOriginal);					// show windows
		cv::imshow("threshImg", threshImg);

		charCheckForEscKey = cv::waitKey(1);					// delay and get key press
	}
	cvReleaseCapture( &capture );
	cvDestroyWindow( "imgOriginal" );
	cvDestroyWindow( "threshImg" ); 

	env[0] = lowR;
	env[1] = highR;
	
	env[2] = lowG;
	env[3] = highG;

	env[4] = lowB;
	env[5] = highB;

	/*std::cout<<"set_env"<<endl;
	std::cout<<"Hue:"<<env[0]<<" - "<<env[1]<<endl;
	std::cout<<"Saturation:"<<env[2]<<" - "<<env[3]<<endl;
	std::cout<<"Value:"<<env[4]<<" - "<<env[5]<<endl;*/

	return env;											
}

int vision::find_balls()
{
	unsigned char key;
	int count = 1;
		
	int lowH = pointer_env[0];							// Set Hue
	int highH = pointer_env[1];

	int lowS = pointer_env[2];							// Set Saturation
	int highS = pointer_env[3];

	int lowV = pointer_env[4];							// Set Value
	int highV = pointer_env[5];

	x_cord = 0.0;
	y_cord = 0.0;
	radius = 0.0;
	
     	//cout<<fileno_str<<"\n"<<endl;
	frame = cvQueryFrame( capture );//get a single frame in IplImage format
	if( !frame ){
		std::cout<<"Could not fetch frame"<<endl;
		//return 0;
	} 
	imgOriginal = cv::cvarrToMat(frame);//convert IplImage to cv::mat
				
		
	//split(imgOriginal, BGRImg);	// Convert Original Image to GrayScal

	cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);	     	// Convert Original Image to GrayScale
	cv::inRange(hsvImg, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), threshImg);
	cv::GaussianBlur(threshImg, threshImg, cv::Size(9, 9), 2);			//Blur Effect
	cv::dilate(threshImg, threshImg, 0);								// Dilate Filter Effect
	cv::erode(threshImg, threshImg, 0);									// Erode Filter Effect
	// fill circles vector with all circles in processed image
	cv::HoughCircles(threshImg,v3fCircles,CV_HOUGH_GRADIENT,2,threshImg.rows / 3,150,50,0,300);  // algorithm for detecting circles	

	for (int i = 0; i < v3fCircles.size(); i++) {				// for each circle
														
		//std::cout << "Ball position X = "<< v3fCircles[i][0] // x position of center point of circle
		//	<<",\tY = "<< v3fCircles[i][1]	 	// y position of center point of circle
		//	<<",\tRadius = "<< v3fCircles[i][2]<< "\n";	// radius of circle

	}	
	
        if(v3fCircles.size()){
		x_cord = v3fCircles[0][0];
		y_cord = v3fCircles[0][1];
		radius = v3fCircles[0][2];
		//circles[0] = v3fCircles[0][0];
		//circles[1] = v3fCircles[0][1];
		//circles[2] = v3fCircles[0][2];
	}
	else{
		cout<<"ball not found"<<"\n";
		x_cord = 0.0;
		y_cord = 0.0;
		radius = 0.0;
		//circles[0] = 0.0;
		//circles[1] = 0.0;
		//circles[2] = 0.0;
	}
	//pointer_circles = circles;

	return 0;//pointer_circles;
}

int vision::locate_ball(GOPI pi)
{
	int count;
	const int halt = 10;
	x_cord = 0.0;
	y_cord = 0.0;
	radius = 0.0;
	capture = cvCaptureFromCAM(0);
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );	
	//usleep(3.0*second);
	find_balls();
	
	count =0;
	while(count < 15 && radius == 0){
		pi.move_bot(halt);
		find_balls();
		count++;
	}

	/*cout <<x_cord<<"\n";
	cout <<y_cord<<"\n";
	cout <<radius<<"\n";*/	
	cvReleaseCapture( &capture );
		
	return 0;
	
}






int find_balls(double* pointer_env, double* x_cord, double* y_cord, double* radius)
{
//Generic variables


	//double* pointer_circles;
	//double circles[3];
	/*std::cout<<"find_balls"<<endl;
	std::cout<<"Hue:"<<pointer_env[0]<<" - "<<pointer_env[1]<<endl;
	std::cout<<"Saturation:"<<pointer_env[2]<<" - "<<pointer_env[3]<<endl;
	std::cout<<"Value:"<<pointer_env[4]<<" - "<<pointer_env[5]<<endl;*/

	unsigned char key;
	int count = 1;
		
	int lowH = pointer_env[0];							// Set Hue
	int highH = pointer_env[1];

	int lowS = pointer_env[2];							// Set Saturation
	int highS = pointer_env[3];

	int lowV = pointer_env[4];							// Set Value
	int highV = pointer_env[5];

	
//Initiate variables for writing the name of file
	//char *fileno_char;
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
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, 640 );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT, 480 );    
     	//cout<<fileno_str<<"\n"<<endl;
	frame = cvQueryFrame( capture );//get a single frame in IplImage format
	if( !frame ){
		std::cout<<"Could not fetch frame"<<endl;
		//return 0;
	} 
	imgOriginal = cv::cvarrToMat(frame);//convert IplImage to cv::mat
				
		
	//split(imgOriginal, BGRImg);	// Convert Original Image to GrayScal

	cv::cvtColor(imgOriginal, hsvImg, CV_BGR2HSV);	     	// Convert Original Image to GrayScale
	cv::inRange(hsvImg, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), threshImg);
	cv::GaussianBlur(threshImg, threshImg, cv::Size(9, 9), 2);			//Blur Effect
	cv::dilate(threshImg, threshImg, 0);								// Dilate Filter Effect
	cv::erode(threshImg, threshImg, 0);									// Erode Filter Effect
	// fill circles vector with all circles in processed image
	cv::HoughCircles(threshImg,v3fCircles,CV_HOUGH_GRADIENT,2,threshImg.rows / 3,150,50,0,300);  // algorithm for detecting circles	

	for (int i = 0; i < v3fCircles.size(); i++) {				// for each circle
														
		std::cout << "Ball position X = "<< v3fCircles[i][0] // x position of center point of circle
			<<",\tY = "<< v3fCircles[i][1]	 	// y position of center point of circle
			<<",\tRadius = "<< v3fCircles[i][2]<< "\n";	// radius of circle

	}	
	
       	cvReleaseCapture( &capture );
        if(v3fCircles.size()){
		*x_cord = v3fCircles[0][0];
		*y_cord = v3fCircles[0][1];
		*radius = v3fCircles[0][2];
		//circles[0] = v3fCircles[0][0];
		//circles[1] = v3fCircles[0][1];
		//circles[2] = v3fCircles[0][2];
	}
	else{
		cout<<"ball not found"<<"\n";
		*x_cord = 0.0;
		*y_cord = 0.0;
		*radius = 0.0;
		//circles[0] = 0.0;
		//circles[1] = 0.0;
		//circles[2] = 0.0;
	}
	//pointer_circles = circles;

	return 0;//pointer_circles;
}

int locate_ball(double* pointer_env,double *x,double *y, double *r)
{
	double x_cord,y_cord,radius;
	int count;
	find_balls(pointer_env,&x_cord,&y_cord,&radius);
	x = &x_cord;
	y = &y_cord;
	r = &radius;
	//cout <<*x<<"\n";
	//cout <<*y<<"\n";
	//cout <<*r<<"\n";

	//cout<<"x_cord = "<<x_cord<<"\n";
	//cout<<"y_cord = "<<y_cord<<"\n";
	//cout<<"radius = "<<radius<<"\n";
	/*double x_cord;
	double y_cord;
	double radius;
	int count;
	x_cord = circles[0];
	y_cord = circles[1];
	radius = circles[2];

	if(radius > 85){
		circles[0] = 0;
		circles[1] = 0;
		circles[2] = 0;		
	}
	if(y_cord<120 && radius > 150){
		circles[0] = 0;
		circles[1] = 0;
		circles[2] = 0;		
	}*/


	count =0;
	
	/*while(count < 2 && circles[0] == 0){
		circles = find_balls(pointer_env);
		count++;
	}*/	


	return 0;
	
	//delete circles;
	//find_balls();
}
int locate_ball(double* pointer_env,double *x,double *y, double *r,GOPI pi)
{
	double x_cord,y_cord,radius;
	int count;
	const int halt = 10;
	find_balls(pointer_env,&x_cord,&y_cord,&radius);
	//cout <<circles[0]<<"\n";
	//cout <<circles[1]<<"\n";
	//cout <<circles[2]<<"\n";
	
	count =0;
	while(count < 3 && radius == 0){
		pi.move_bot(halt);
		find_balls(pointer_env,&x_cord,&y_cord,&radius);
		count++;
	}

	//cout <<x_cord<<"\n";
	//cout <<y_cord<<"\n";
	//cout <<radius<<"\n";	

	*x = x_cord;
	*y = y_cord;
	*r = radius;

	//cout <<"x = "<<x<<"\n";
	//cout <<"y = "<<y<<"\n";
	//cout <<"r = "<<r<<"\n";	
	

	//cout <<"*x = "<<*x<<"\n";
	//cout <<"*y = "<<*y<<"\n";
	//cout <<"*r = "<<*r<<"\n";	
	
	return 0;
	
	//delete circles;
	//find_balls();
}
