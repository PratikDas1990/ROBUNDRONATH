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
              cout << "signal = ",signal;
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

void UDPcomms::server()
{
    unsigned short servPort = atoi("2000");//(argv[1]); // First arg:  local port

    namedWindow("recv", WINDOW_AUTOSIZE);
    try {
        UDPSocket sock(servPort);

        char buffer[BUF_LEN]; // Buffer for echo string
        int recvMsgSize; // Size of received message
        string sourceAddress; // Address of datagram source
        unsigned short sourcePort; // Port of datagram source

        clock_t last_cycle = clock();

        while (1) {
            // Block until receive message from a client
            do {
                recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
            } while (recvMsgSize > sizeof(int));
            int total_pack = ((int * ) buffer)[0];

            cout << "expecting length of packs:" << total_pack << endl;
            char * longbuf = new char[PACK_SIZE * total_pack];
            for (int i = 0; i < total_pack; i++) {
                recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
                if (recvMsgSize != PACK_SIZE) {
                    cerr << "Received unexpected size pack:" << recvMsgSize << endl;
                    continue;
                }
                memcpy( & longbuf[i * PACK_SIZE], buffer, PACK_SIZE);
            }

            cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;
 
            Mat rawData = Mat(1, PACK_SIZE * total_pack, CV_8UC1, longbuf);
            Mat frame = imdecode(rawData, IMREAD_COLOR);
            if (frame.size().width == 0) {
                cerr << "decode failure!" << endl;
                continue;
            }
            imshow("recv", frame);
            free(longbuf);

            waitKey(1);
            clock_t next_cycle = clock();
            double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
            cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;

            cout << next_cycle - last_cycle;
            last_cycle = next_cycle;
        }
    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }
};
void UDPcomms::serverDNN()
{
    unsigned short servPort = atoi("2000");//(argv[1]); // First arg:  local port

    namedWindow("recv", WINDOW_AUTOSIZE);
    try {
        UDPSocket sock(servPort);

        char buffer[BUF_LEN]; // Buffer for echo string
        int recvMsgSize; // Size of received message
        string sourceAddress; // Address of datagram source
        unsigned short sourcePort; // Port of datagram source
	int object_class[16];
        int count;   
	int buffer_size = 16*sizeof(int);
 
        for(int i = 0 ; i < 13 ; i++)
		object_class[i] = 0;
        object_class[0] = 444;

        clock_t last_cycle = clock();
        //Define DNN Model
	CV_TRACE_FUNCTION();
	String modelTxt = "MobileNetSSD_deploy.prototxt";
	String modelBin = "MobileNetSSD_deploy.caffemodel";
    
	//String imageFile = (argc > 1) ? argv[1] : "test.jpg";
	Net net = dnn::readNetFromCaffe(modelTxt, modelBin);
	if (net.empty())
	{
	        std::cerr << "Can't load network by using the following files: " << std::endl;
	        std::cerr << "prototxt:   " << modelTxt << std::endl;
        	std::cerr << "caffemodel: " << modelBin << std::endl;
        	exit(-1);
	}	
        //END DNN model definition

        while (1) {
            // Block until receive message from a client
            count = 0;
            do {
                recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
            } while (recvMsgSize > sizeof(int));
            int total_pack = ((int * ) buffer)[0];

            cout << "expecting length of packs:" << total_pack << endl;
            char * longbuf = new char[PACK_SIZE * total_pack];
            for (int i = 0; i < total_pack; i++) {
                recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
                if (recvMsgSize != PACK_SIZE) {
                    cerr << "Received unexpected size pack:" << recvMsgSize << endl;
                    continue;
                }
                memcpy( & longbuf[i * PACK_SIZE], buffer, PACK_SIZE);
            }

            cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;
 
            Mat rawData = Mat(1, PACK_SIZE * total_pack, CV_8UC1, longbuf);
            Mat frame = imdecode(rawData, IMREAD_COLOR);
            if (frame.size().width == 0) {
                cerr << "decode failure!" << endl;
                continue;
            }
            //imshow("recv", frame);


		Mat img2;
		resize(frame, img2, Size(240,240));
		Mat inputBlob = blobFromImage(img2, 0.007843, Size(240,240), Scalar(127.5, 127.5, 127.5), false);

		net.setInput(inputBlob, "data");
		Mat detection = net.forward("detection_out");
		Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

		ostringstream ss;
		float confidenceThreshold = 0.2;
                //idx_class[0] = detectionMat.rows;
		cout<<"No. of objects = "<< detectionMat.rows<<endl;
		//int signal = 444;
		//sock.sendTo(& signal,sizeof(int), sourceAddress, sourcePort);
                //sock.sendTo(& detectionMat.rows,sizeof(int), sourceAddress, sourcePort);
		for (int i = 0; i < detectionMat.rows ; i++)
		{
			float confidence = detectionMat.at<float>(i, 2);
			if (confidence > confidenceThreshold)
			{
				count = count + 1;
				int idx = static_cast<int>(detectionMat.at<float>(i, 1));
				int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
				int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
				int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
				int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);
                                if(count < 16)
				{
					object_class[count]     = idx;
					object_class[count + 1] = xLeftBottom;
					object_class[count + 2] = yLeftBottom;
					object_class[count + 3] = xRightTop;
					object_class[count + 4] = yRightTop;
        	                        count = count + 5;
				}

				Rect object((int)xLeftBottom, (int)yLeftBottom,(int)(xRightTop - xLeftBottom),(int)(yRightTop - yLeftBottom));

				rectangle(frame, object, Scalar(0, 255, 0), 2);

				cout << i<<">"<<CLASSES[object_class[count - 5]] << ": " << confidence << endl;
                                cout << "Loc : "<<object_class[count - 4]
				<<","<<object_class[count - 3]
				<<","<<object_class[count - 2]
				<<","<<object_class[count - 1]
				<<endl;

				//idx_class[i+1] = idx;
				//signal = 555;
				//sock.sendTo(& signal,sizeof(int), sourceAddress, sourcePort);
				//for (int i = 0; i < 5; i++)
				//	sock.sendTo(& obj[i],sizeof(int), sourceAddress, sourcePort);
				/*sock.sendTo(&idx, sizeof(int), sourceAddress, sourcePort);
				sock.sendTo(&xLeftBottom, sizeof(int), sourceAddress, sourcePort);
				sock.sendTo(&yLeftBottom, sizeof(int), sourceAddress, sourcePort);
				sock.sendTo(&xRightTop, sizeof(int), sourceAddress, sourcePort);
				sock.sendTo(&yRightTop, sizeof(int), sourceAddress, sourcePort);*/

				ss.str("");
				ss << confidence;
				String conf(ss.str());
				String label = CLASSES[idx] + ": " + conf;
				int baseLine = 0;
				Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
				putText(frame, label, Point(xLeftBottom, yLeftBottom),
				FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0,0));
			}
		}
		//cout<<"classes"<<idx_class[1]<<","<<idx_class[2]<<endl;
                sock.sendTo(& object_class[0], buffer_size, sourceAddress, sourcePort);
		imshow("Live",frame);
		int key = cv::waitKey(5);
		key = (key==255) ? -1 : key; //#Solve bug in 3.2.0

		waitKey(1);
		clock_t next_cycle = clock();
		double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
		//cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;
		free(longbuf);
		//cout << next_cycle - last_cycle;
		last_cycle = next_cycle;
		if (key>=0)
			break;            
        }
    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }
};
