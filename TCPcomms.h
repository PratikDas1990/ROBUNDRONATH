#ifndef TCPCOMMS_H
#define TCPCOMMS_H
#include <stdio.h> 

#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include<iostream>
#define PORT 8080 

using namespace std;

class TCPcomms
{
private:
//server variables
	int server_fd_server;
	int socket_server;
	int valread_server;
	struct sockaddr_in address_server;
	int opt = 1; 
	int addrlen_server = sizeof(address_server);
	//int setup_server();
//client variables

	int sock_client = 0, valread_client; 
	struct sockaddr_in serv_addr_client; 
public:
	int setup_server();
	int recv_data_server[15];
	int send_data_server[15];
	int grab_server();
	int send_server();

	int setup_client();
	int send_data_client[15];
	int recv_data_client[15];
	int grab_client();
	int send_client();

};
#endif
