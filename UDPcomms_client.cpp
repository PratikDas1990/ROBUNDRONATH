#include <fstream>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <termios.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/utils/trace.hpp>
#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include "UDPcomms.h"
#include "config.h"
#define BUF_LEN 65540 // Larger than maximum UDP packet size

using namespace cv;
using namespace cv::dnn;
using namespace std;

string CLASSES[] = {"background", "aeroplane", "bicycle", "bird", "boat",
	"bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
	"dog", "horse", "motorbike", "person", "pottedplant", "sheep",
	"sofa", "train", "tvmonitor"};


void UDPcomms::client()
{
    char buffer_class[BUF_LEN];
    int idx[5];
    int buffer_size = 16*sizeof(int);

    string servAddress = "192.168.0.20";//argv[1]; // First arg: server address
    unsigned short servPort = Socket::resolveService("2000","udp");//(argv[2], "udp");

    try {
        UDPSocket sock;
        int jpegqual =  ENCODE_QUALITY; // Compression Parameter

        Mat frame, send;
        vector < uchar > encoded;
        VideoCapture cap(0); // Grab the camera
        //namedWindow("send", WINDOW_AUTOSIZE);
        if (!cap.isOpened()) {
            cerr << "OpenCV Failed to open camera";
            exit(1);
        }

        clock_t last_cycle = clock();
        while (1) {
            cap >> frame;
            if(frame.size().width==0)continue;//simple integrity check; skip erroneous data...
            resize(frame, send, Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, INTER_LINEAR);
            vector < int > compression_params;
            compression_params.push_back(IMWRITE_JPEG_QUALITY);
            compression_params.push_back(jpegqual);

            imencode(".jpg", send, encoded, compression_params);
            //imshow("send", send);
            int total_pack = 1 + (encoded.size() - 1) / PACK_SIZE;

            int ibuf[1];
            ibuf[0] = total_pack;
            sock.sendTo(ibuf, sizeof(int), servAddress, servPort);
            //cout << "encoded.size() = "<<encoded.size()<<endl;       
	    //cout << "PACK_SIZE = "<<PACK_SIZE<<endl;
	    //cout << "sizeof(int) = "<<sizeof(int)<<endl;  
            for (int i = 0; i < total_pack; i++)
                sock.sendTo( & encoded[i * PACK_SIZE], PACK_SIZE, servAddress, servPort);
   
            int signal = 0;
            sock.recvFrom(buffer_class, BUF_LEN, servAddress, servPort);
            signal = ((int * ) buffer_class)[0];
	    if(signal == 444){
              cout << "signal = "<<signal<<endl;
            }
               //sock.recvFrom(buffer_class, sizeof(int), servAddress, servPort);
       	       //int total_objects = ((int * ) buffer_class)[0];
               //cout << "total_objects = "<<total_objects<<endl;
	       /*if(total_objects > 0 && total_objects < 10){
		 signal = 0;
                 sock.recvFrom(buffer_class, sizeof(int), servAddress, servPort);
                 signal = ((int * ) buffer_class)[0];
                 if(signal == 555){
	           for (int i = 0; i < 5 ; i++) {
       	             sock.recvFrom(buffer_class, sizeof(int), servAddress, servPort);
       	             idx[i] = ((int * ) buffer_class)[0];
      	           }
                 }
	         cout << endl <<CLASSES[idx[0]] <<":" << idx[1]<<","<<idx[2]<<","<<idx[3]<<","<<idx[4]<<endl;
               }
	     }*/

            /*sock.recvFrom(idx_class,BUF_LEN, servAddress, servPort);
	    int idx = ((int * ) idx_class)[0];
            sock.recvFrom(idx_class,BUF_LEN, servAddress, servPort);
	    int xLeftBottom = ((int * ) idx_class)[0];
            sock.recvFrom(idx_class,BUF_LEN, servAddress, servPort);
	    int yLeftBottom = ((int * ) idx_class)[0];
            sock.recvFrom(idx_class,BUF_LEN, servAddress, servPort);
	    int xRightTop = ((int * ) idx_class)[0];
	    sock.recvFrom(idx_class,BUF_LEN, servAddress, servPort);
	    int yRightTop = ((int * ) idx_class)[0];*/

	    
            //sock.recvFrom(idx_class,BUF_LEN, servAddress, servPort);
	    //int total_objects = ((int * ) idx_class)[0];
	    //cout << "total objects = "<<total_objects << endl;
            //cout << "object1 = "<<((int * ) idx_class)[1]<<endl;
            //cout << "object1 = "<<((int * ) idx_class)[2]<<endl; 
            waitKey(FRAME_INTERVAL*20);

            clock_t next_cycle = clock();
            double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
            //cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;

            //cout << next_cycle - last_cycle;
            last_cycle = next_cycle;
        }
        // Destructor closes the socket

    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }

};


