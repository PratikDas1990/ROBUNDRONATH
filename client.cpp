
// Client side C/C++ program to demonstrate Socket programming 
#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include <iostream>               // For cout and cerr
#include <cstdlib>                // For atoi()
#include "opencv2/opencv.hpp"
#include "UDPcomms.h"
#include "config.h"
#include "TCPcomms.h"
#include <fstream>
using namespace std;
int main(int argc, char const *argv[]) 
{ 
	UDPcomms commDATA;
	commDATA.sendDATA();
	return 0;
} 
