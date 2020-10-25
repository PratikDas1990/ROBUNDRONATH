
// Server side C/C++ program to demonstrate Socket programming 
#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include <iostream>               // For cout and cerr
#include <cstdlib>                // For atoi()
#include "opencv2/opencv.hpp"
#include "UDPcomms.h"
#include "config.h"
#include "TCPcomms.h"
#include <fstream>
//string CLASSES[] = {"background", "aeroplane", "bicycle", "bird", "boat",
//      "bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
//      "dog", "horse", "motorbike", "person", "pottedplant", "sheep",
//      "sofa", "train", "tvmonitor"};

using namespace std;
int main(int argc, char const *argv[]) 
{ 
	string CLASSES[] = {"background", "aeroplane", "bicycle", "bird", "boat",
      		"bottle", "bus", "car", "cat", "chair", "cow", "diningtable",
      		"dog", "horse", "motorbike", "person", "pottedplant", "sheep",
      		"sofa", "train", "tvmonitor"};
	
	UDPcommsfetch commDATA;
//	while(1){
		commDATA.fetchDATA();
	    	cout << "first_object:" << commDATA.object_class[0] <<" = "<<CLASSES[commDATA.object_class[0]]
		<<"; x1 = "<<commDATA.object_class[1]
		<<"; y1 = "<<commDATA.object_class[2]
		<<"; x2 = "<<commDATA.object_class[3]
		<<"; y2 = "<<commDATA.object_class[4]
		<< endl;
            	cout << "second_object:" << commDATA.object_class[5] <<" = "<<CLASSES[commDATA.object_class[5]]
		<<"; x1 = "<<commDATA.object_class[6]
		<<"; y1 = "<<commDATA.object_class[7]
		<<"; x2 = "<<commDATA.object_class[8]
		<<"; y2 = "<<commDATA.object_class[9]
		<< endl;  
          	cout << "third_object:" << commDATA.object_class[10] <<" = "<<CLASSES[commDATA.object_class[10]]
		<<"; x1 = "<<commDATA.object_class[11]
		<<"; y1 = "<<commDATA.object_class[12]
		<<"; x2 = "<<commDATA.object_class[13]
		<<"; y2 = "<<commDATA.object_class[14]
		<< endl;
//	}
	return 0;
} 

