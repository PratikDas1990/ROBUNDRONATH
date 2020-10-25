#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include<iostream>
#include "TCPcomms.h"
#define PORT 8080 

using namespace std;

int TCPcomms::setup_server(){
if ((server_fd_server = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd_server, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address_server.sin_family = AF_INET; 
    address_server.sin_addr.s_addr = INADDR_ANY; 
    address_server.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd_server, (struct sockaddr *)&address_server,  
                                 sizeof(address_server))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd_server, 10) < 0) //3
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((socket_server = accept(server_fd_server, (struct sockaddr *)&address_server,  
                       (socklen_t*)&addrlen_server))<0) 
    { 
        perror("accept"); 
        exit(EXIT_FAILURE); 
    } 
    return 0;	
}

int TCPcomms::grab_server(){
    //valread_server = recv( socket_server , &recv_data_server, 1024,MSG_WAITALL);
    valread_server = read( socket_server , &recv_data_server, 1024);
    return 0; 
}
int TCPcomms::send_server(){
    send(socket_server , &send_data_server , sizeof(send_data_server) , 0 );
    return 0; 
}
//**************************************************************************************************
int TCPcomms::setup_client(){
    if ((sock_client = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr_client.sin_family = AF_INET; 
    serv_addr_client.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "192.168.0.20", &serv_addr_client.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock_client, (struct sockaddr *)&serv_addr_client, sizeof(serv_addr_client)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    return 0;	
}
int TCPcomms::grab_client(){
    //valread_server = recv( sock_client , &recv_data_client, 1024,MSG_WAITALL);
    valread_client = read( sock_client , &recv_data_client, 1024);
    return 0; 
}

int TCPcomms::send_client(){
    send(sock_client , &send_data_client , sizeof(send_data_client) , 0 );
    return 0; 
}





