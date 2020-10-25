#ifndef UDPCOMMS_H
#define UDPCOMMS_H
#include "PracticalSocket.h"      // For UDPSocket and SocketException
#include <iostream>               // For cout and cerr
#include <cstdlib>                // For atoi()
#include "opencv2/opencv.hpp"
#include "config.h"
#include "TCPcomms.h"
using namespace std;
using namespace cv;

class UDPcomms
{
private:
	int read_class[20];


public:
	void client();
        void sendDATA();
	void client_single();
	void server();
	void fetchDATAtest();
        void serverDNN();
	void server_test();
        int object_class[15];
};
class UDPcommsfetch
{
private:
        unsigned short servPort_fetch = atoi("3000");//(argv[1]); // First arg:  local port
        UDPSocket* sock_fetch = new UDPSocket(servPort_fetch);
        char buffer_fetch[BUF_LEN]; // Buffer for echo string
        int recvMsgSize_fetch; // Size of received message
        string sourceAddress_fetch; // Address of datagram source
        unsigned short sourcePort_fetch; // Port of datagram source
public:
	void fetchDATA();
        int object_class[15];

};
#endif
