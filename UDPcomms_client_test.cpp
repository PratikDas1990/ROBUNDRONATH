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

#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 8080 
#define BUF_LEN 65540 // Larger than maximum UDP packet size

using namespace cv;
using namespace cv::dnn;
using namespace std;


void UDPcomms::client()
{
    int object_class[15];
//TCP protocol for client setup*******************************************************
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    int buffer_array[16];
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        //return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "192.168.0.20", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        //return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        //return -1; 
    } 
     valread = read(  sock , &buffer_array, 1024);

//************************************************************************************

   // string servAddress = "192.168.43.194";//argv[1]; // First arg: server address
    string servAddress = "192.168.0.20";//a
    unsigned short servPort = Socket::resolveService("2000","udp");//(argv[2], "udp");

    try {
        UDPSocket sock;
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
	    cout << "total_pack = "<<total_pack<<endl;
            for (int i = 0; i < total_pack; i++){
		//cout<<"sending packet no: = "<<i<<endl;
                sock.sendTo( & encoded[i * PACK_SIZE], PACK_SIZE, servAddress, servPort);
	    }
   
	    
            waitKey(FRAME_INTERVAL);
	    valread = read( sock , &buffer_array, 1024); 
        	cout << "object1 = "<<CLASSES[buffer_array[1]]<<endl;
            	cout << "loc = "<< buffer_array[2]<<endl; 
            	cout << "loc = "<< buffer_array[3]<<endl;
            	cout << "loc = "<< buffer_array[4]<<endl; 
            	cout << "loc = "<< buffer_array[5]<<endl; 
            	cout << "object2 = "<<CLASSES[ buffer_array[6]]<<endl;
            	cout << "loc = "<< buffer_array[7]<<endl; 
            	cout << "loc = "<< buffer_array[8]<<endl;
            	cout << "loc = "<< buffer_array[9]<<endl; 
            	cout << "loc = "<< buffer_array[10]<<endl; 
            	cout << "object3 = "<<CLASSES[buffer_array[11]]<<endl;
            	cout << "loc = "<< buffer_array[12]<<endl; 
            	cout << "loc = "<< buffer_array[13]<<endl;
            	cout << "loc = "<< buffer_array[14]<<endl; 
           	cout << "loc = "<< buffer_array[15]<<endl; 
	    system("scp pratik@192.168.0.20:~/AutoBot18/object.txt ./");
            fstream file;
            int x;
            file.open("object.txt");
            int i = 0;
            while (file>>x) {
                object_class[i] = x;
                i++;
            }
            file.close();
        	cout << "object1 = "<<CLASSES[object_class[0]]<<endl;
            	cout << "loc = "<< object_class[1]<<endl; 
            	cout << "loc = "<< object_class[2]<<endl;
            	cout << "loc = "<< object_class[3]<<endl; 
            	cout << "loc = "<< object_class[4]<<endl; 
            	cout << "object2 = "<<CLASSES[ object_class[5]]<<endl;
            	cout << "loc = "<< object_class[6]<<endl; 
            	cout << "loc = "<< object_class[7]<<endl;
            	cout << "loc = "<< object_class[8]<<endl; 
            	cout << "loc = "<< object_class[9]<<endl; 
            	cout << "object3 = "<<CLASSES[object_class[10]]<<endl;
            	cout << "loc = "<< object_class[11]<<endl; 
            	cout << "loc = "<< object_class[12]<<endl;
            	cout << "loc = "<< object_class[13]<<endl; 
           	cout << "loc = "<< object_class[14]<<endl; 
            
            //clock_t next_cycle = clock();
            //double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
            //cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;

            //cout << next_cycle - last_cycle;
            //last_cycle = next_cycle;
        }
        // Destructor closes the socket

    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }

};

void UDPcomms::client_single()
{

   // string servAddress = "192.168.43.194";//argv[1]; // First arg: server address
    string servAddress = "192.168.0.20";//a
    unsigned short servPort = Socket::resolveService("2000","udp");//(argv[2], "udp");

    try {
        UDPSocket sock;
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

    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }

};


