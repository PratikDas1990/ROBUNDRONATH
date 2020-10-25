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
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8080
#define BUF_LEN 65540 // Larger than maximum UDP packet size

using namespace cv;
using namespace cv::dnn;
using namespace std;


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
		//cout<<"grabbing packet no.:"<<i<<endl;
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
//TCP protocol setup*************************************************************
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0}; 
    int data_from_client[3] = {0,0,0};
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                       (socklen_t*)&addrlen))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
//*****************************************************************************
    try {
        UDPSocket sock(servPort);

        char buffer[BUF_LEN]; // Buffer for echo string
        int recvMsgSize; // Size of received message
        string sourceAddress; // Address of datagram source
        unsigned short sourcePort; // Port of datagram source
	int object_class[16];
        int count;   
	int buffer_size = 16*sizeof(int);
 
        for(int i = 0 ; i < 16 ; i++)
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
            for(int i = 1 ; i < 16 ; i++)
			object_class[i] = 0;
            do {
                recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
            } while (recvMsgSize > sizeof(int));
            int total_pack = ((int * ) buffer)[0];

            cout << "expecting length of packs:" << total_pack << endl;
            char * longbuf = new char[PACK_SIZE * total_pack];
            for (int i = 0; i < total_pack; i++) {
		//cout<<"grabbing packet no.:"<<i<<endl;
                recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
                if (recvMsgSize != PACK_SIZE) {
                    cerr << "Received unexpected size pack:" << recvMsgSize << endl;
                    continue;
                }
                memcpy( & longbuf[i * PACK_SIZE], buffer, PACK_SIZE);
            }

            cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;
            system("rm object.txt");//delete the object class file 
            Mat rawData = Mat(1, PACK_SIZE * total_pack, CV_8UC1, longbuf);
            Mat frame = imdecode(rawData, IMREAD_COLOR);
            if (frame.size().width == 0) {
                cerr << "decode failure!" << endl;
                continue;
            }


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
                                if(i < 3)
				{
					object_class[count]     = idx;
					object_class[count + 1] = xLeftBottom;
					object_class[count + 2] = yLeftBottom;
					object_class[count + 3] = xRightTop;
					object_class[count + 4] = yRightTop;
        	                        count = count + 4;
					cout << i<<">"<<CLASSES[object_class[count - 4]] << ": " << confidence << endl;
	                                cout << "Loc : "<<object_class[count - 3]
					<<","<<object_class[count - 2]
					<<","<<object_class[count - 1]
					<<","<<object_class[count - 0]
					<<endl;
				}
				//cout << i<<">"<<CLASSES[idx] << ": " << confidence << endl;

				Rect object((int)xLeftBottom, (int)yLeftBottom,(int)(xRightTop - xLeftBottom),(int)(yRightTop - yLeftBottom));

				rectangle(frame, object, Scalar(0, 255, 0), 2);

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
                send(new_socket , &object_class , sizeof(object_class) , 0 );
                valread = read( new_socket , &object_class, 1024);
                //sock.sendTo(& object_class[0], buffer_size, sourceAddress, sourcePort);
  		ofstream myfile;
  		myfile.open ("object.txt");
  		myfile << object_class[1]<<"\n"<< object_class[2]<<"\n"
			<< object_class[3]<<"\n"<< object_class[4]<<"\n"
			<< object_class[5]<<endl;
                myfile << object_class[6]<<"\n"<< object_class[7]<<"\n"
                        << object_class[8]<<"\n"<< object_class[9]<<"\n"
                        << object_class[10]<<endl;
                myfile << object_class[11]<<"\n"<< object_class[12]<<"\n"
                        << object_class[13]<<"\n"<< object_class[14]<<"\n"
                        << object_class[15]<<endl;
		
  		myfile.close();                
                //system("scp object.txt root@192.168.0.100:/root/AutoBot18/");
		//
		imshow("Live",frame);
		int key = cv::waitKey(5);
		//key = (key==255) ? -1 : key; //#Solve bug in 3.2.0

		//waitKey(1);
		clock_t next_cycle = clock();
		double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
		//cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;
		free(longbuf);
		cout << "time taken :" <<next_cycle - last_cycle<<endl;
		last_cycle = next_cycle;
		if (key>=0)
			break;            
        }
    } catch (SocketException & e) {
        cerr << e.what() << endl;
        exit(1);
    }
};
