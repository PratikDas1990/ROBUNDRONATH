#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include <iostream>               // For cout and cerr
#include <cstdlib>                // For atoi()
#include "opencv2/opencv.hpp"
#include "UDPcomms.h"
#include "config.h"


using namespace std;
using namespace cv;

int main(void)
{	
	UDPcomms comm;
	comm.serverDNN();
	return 0;
}
