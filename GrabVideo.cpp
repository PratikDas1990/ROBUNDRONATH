#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/mytest.hpp> 
using namespace cv;
using namespace std;
 
int main(int argc,char ** argv)
{
	
	cv::Mat imgOriginal;		// Input image
	VideoCapture cap(0);
  
	cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
	if (!cap.isOpened()) {
		cerr << "ERROR: Unable to open the camera" << endl;
		return 0;
	}
 
	cout << "Start grabbing, press a key on Live window to terminate" << endl;
	while(1) {
		cap >> imgOriginal;
		if (imgOriginal.empty()) {
			cerr << "ERROR: Unable to grab from the camera" << endl;
			break;
		}
		cv::namedWindow("imgOriginal", CV_WINDOW_NORMAL);
		cv::imshow("imgOriginal", imgOriginal);					// show windows
		int key = cv::waitKey(5);
		key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
		if (key>=0)
		break;
  }
  cvDestroyWindow( "imgOriginal" );
 
  cout << "Closing the camera" << endl;
  cap.release();
  destroyAllWindows();
  cout << "bye!" <<endl;
  return 0;
}
