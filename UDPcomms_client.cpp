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
#include <vector> 

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
#include "TCPcomms.h"
#define BUF_LEN 65540 // Larger than maximum UDP packet size
string CLASSES[] = {"background", "aeroplane", "bicycle", "bird", "boat",
      "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
      "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
      "sofa", "train", "tvmonitor"};

using namespace cv;
using namespace cv::dnn;
using namespace std;


void UDPcomms::client()
{
    int object_class[15];

    string servAddress = "192.168.43.194";//argv[1]; // First arg: server address
   // string servAddress = "192.168.0.20";//a
    unsigned short servPort = Socket::resolveService("2000","udp");//(argv[2], "udp");

    try {
        UDPSocket sock;
	//TCPcomms comm_client;
	//comm_client.setup_client();
        int jpegqual =  ENCODE_QUALITY; // Compression Paramete
	int recvMsgSize; 
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
            if(frame.size().width==0)continue;//continue;//simple integrity check; skip erroneous data...
            resize(frame, send, Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, INTER_LINEAR);
            vector < int > compression_params;
            compression_params.push_back(IMWRITE_JPEG_QUALITY);
            compression_params.push_back(jpegqual);

            imencode(".jpg", send, encoded, compression_params);
            int total_pack = 1 + (encoded.size() - 1) / PACK_SIZE;

            int ibuf[1];
            ibuf[0] = total_pack;
            sock.sendTo(ibuf, sizeof(int), servAddress, servPort);
            //cout << "encoded.size() = "<<encoded.size()<<endl;       
	    //cout << "PACK_SIZE = "<<PACK_SIZE<<endl;
	    //cout << "total_pack = "<<total_pack<<endl;
            for (int i = 0; i < total_pack; i++){
		//cout<<"sending packet no: = "<<i<<endl;
                sock.sendTo( & encoded[i * PACK_SIZE], PACK_SIZE, servAddress, servPort);
	    }
   
	    
            waitKey(FRAME_INTERVAL);
	    //comm_client.grab_client();
	    //object_class[0]=comm_client.recv_data_client[0];
	    //object_class[5]=comm_client.recv_data_client[1];
            //object_class[10]=comm_client.recv_data_client[2];
	    /*system("scp pratik@192.168.0.20:~/AutoBot18/object.txt ./");
            fstream file;
            int x;
            file.open("object.txt");
            int i = 0;
            while (file>>x) {
                object_class[i] = x;
                i++;
            }
            file.close();*/
        	//cout << "object1 = "<<CLASSES[object_class[0]]<<endl;
            	//cout << "loc = "<< object_class[1]<<endl; 
            	//cout << "loc = "<< object_class[2]<<endl;
            	//cout << "loc = "<< object_class[3]<<endl; 
            	//cout << "loc = "<< object_class[4]<<endl; 
            	//cout << "object2 = "<<CLASSES[ object_class[5]]<<endl;
            	//cout << "loc = "<< object_class[6]<<endl; 
            	//cout << "loc = "<< object_class[7]<<endl;
            	//cout << "loc = "<< object_class[8]<<endl; 
            	//cout << "loc = "<< object_class[9]<<endl; 
            	//cout << "object3 = "<<CLASSES[object_class[10]]<<endl;
            	//cout << "loc = "<< object_class[11]<<endl; 
            	//cout << "loc = "<< object_class[12]<<endl;
            	//cout << "loc = "<< object_class[13]<<endl; 
           	//cout << "loc = "<< object_class[14]<<endl; 
         
            clock_t next_cycle = clock();
            //double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
            //cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;

            //cout << next_cycle - last_cycle<<endl;
            last_cycle = next_cycle;
        }
        // Destructor closes the socket

    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }

};
//***********************************************************************************************

//***********************************************************************************************
void UDPcomms::client_single()
{
    int object_class[15];
    string servAddress = "192.168.43.194";//argv[1]; // First arg: server address
    //string servAddress = "192.168.0.20";//a
    unsigned short servPort = Socket::resolveService("2000","udp");//(argv[2], "udp");

    try {
        UDPSocket sock;
	TCPcomms comm_client;
	comm_client.setup_client();

        int jpegqual =  ENCODE_QUALITY; // Compression Paramete
	int recvMsgSize; 
        Mat frame, send;
        vector < uchar > encoded;
        VideoCapture cap(0); // Grab the camera
        if (!cap.isOpened()) {
            cerr << "OpenCV Failed to open camera";
            exit(1);
        }

        clock_t last_cycle = clock();
        cap >> frame;
        if(frame.size().width==0)exit(1);//continue;//simple integrity check; skip erroneous data...
        resize(frame, send, Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, INTER_LINEAR);
        vector < int > compression_params;
        compression_params.push_back(IMWRITE_JPEG_QUALITY);
        compression_params.push_back(jpegqual);

        imencode(".jpg", send, encoded, compression_params);
        int total_pack = 1 + (encoded.size() - 1) / PACK_SIZE;

        int ibuf[1];
        ibuf[0] = total_pack;
        sock.sendTo(ibuf, sizeof(int), servAddress, servPort);
        //cout << "encoded.size() = "<<encoded.size()<<endl;       
	//cout << "PACK_SIZE = "<<PACK_SIZE<<endl;
	//cout << "total_pack = "<<total_pack<<endl;
        for (int i = 0; i < total_pack; i++)
                sock.sendTo( & encoded[i * PACK_SIZE], PACK_SIZE, servAddress, servPort);
   
            //waitKey(FRAME_INTERVAL);
	    comm_client.grab_client();
	    object_class[0]=comm_client.recv_data_client[0];
	    object_class[5]=comm_client.recv_data_client[1];
            object_class[10]=comm_client.recv_data_client[2];
        	cout << "object1 = "<<CLASSES[object_class[0]]<<endl;
            	cout << "object2 = "<<CLASSES[ object_class[5]]<<endl;
            	cout << "object3 = "<<CLASSES[object_class[10]]<<endl;

    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }

};

//***********************************************************************************************

//***********************************************************************************************
void UDPcomms::sendDATA()
{
    int x;
    fstream file;
    string servAddress = "192.168.43.193";//argv[1]; // First arg: server address
   // string servAddress = "192.168.0.100";
    unsigned short servPort = Socket::resolveService("3000","udp");//(argv[2], "udp");

    try {
        UDPSocket sock;
        while (1) {
            x = 0;
            //int objs = 0;
            file.open("object.txt");
            //while (file>>x) {
            for(int objs = 0; objs < 15 ; objs ++){
                file>>x;
	        read_class[objs] = x;
		cout<<objs<<" = "<<x<<":"<<read_class[objs]<<endl;
                //objs++;   
                
            };
            file.close();
            //sock.sendTo( ibuf, sizeof(int), servAddress, servPort);
             cout<<"before sending data to server"<<endl;
            sock.sendTo( & read_class[0], 15*sizeof(int), servAddress, servPort);
            cout<<"sent data to server"<<endl;
            //free(&object_class[0]);
        }
        // Destructor closes the socket

    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }

};


