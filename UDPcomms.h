#ifndef UDPCOMMS_H
#define UDPCOMMS_H
#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include <iostream>               // For cout and cerr
#include <cstdlib>                // For atoi()
#include "opencv2/opencv.hpp"
#include "config.h"

using namespace std;
using namespace cv;

class UDPcomms
{
public:
	void client();
	void server();
        void serverDNN();
};
#endif
