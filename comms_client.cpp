#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include <iostream>               // For cout and cerr
#include <cstdlib>                // For atoi()
#include "opencv2/opencv.hpp"
#include "UDPcomms.h"
#include "config.h"
#include <fstream>

using namespace std;
using namespace cv;

int main(void)
{	
	string CLASSES[] = {"background", "aeroplane", "bicycle", "bird", "boat",
      "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
      "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
      "sofa", "train", "tvmonitor"};

	clock_t start = clock();
	UDPcomms comm;
	comm.client();
//	comm.client_single();
	//Read from object.txt
        system("scp pratik@192.168.152.15:~/AutoBot18/object.txt ./");
        fstream file;
        int x;
	int object_class[15];
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
        
	clock_t end = clock();
	cout << "total time : "<<(end - start)<<endl;
	//system("pause");
	return 0;
}
