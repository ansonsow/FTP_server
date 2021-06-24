/**
 * @file: ftp_server_net_util.cpp
 * @author: Anson SU 578677684 F20N03, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
 *
 */

#include "../include/ftp_server_net_util.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
void closeSocket(int& sockDescriptor){
//Closes the stream socket, represented by 'sockDescriptor'.
  close(sockDescriptor);
}

int getPortFromSocketDescriptor(const int sockDescriptor){
//Determines and returns the associated port number from a given socket descriptor.
  struct sockaddr_in addr;
  socklen_t len = sizeof(addr);
  getsockname(sockDescriptor, (struct sockaddr*)&addr, &len);
  int portN= ntohs(addr.sin_port);
  return portN;
}
bool isSocketReadyToRead(const int sockDescriptor, const int timeoutSec, const int timeoutUSec, bool& isError, bool& isTimedout){
//Returns true if there is any data sent by the remote computer on the stream socket represented by 'sockDescriptor'.
//Waits for the data until 'timeoutSec' + 0.000001x'timeoutUsec' time.
//If no data has been sent before the time out, sets 'isTimedout' value to 'true'.
//If any error occurs, sets 'isError' value to 'true'.
  fd_set readfds;
  struct timeval tv;
  //  char arr[1024];
  tv.tv_sec = timeoutSec;
  tv.tv_usec=timeoutUSec;
  FD_ZERO(&readfds);
  FD_SET(sockDescriptor, &readfds);
  int rec = select(sockDescriptor+1,&readfds,NULL,NULL,&tv);
  if(rec==-1){
    isError = true;
    return false;
  }else if(rec ==0){
    isTimedout = true;
    return false;
  }else{
    //bzero(arr,sizeof(arr));
    //read(sockDescriptor,arr,sizeof(arr));
    isError = false;
    isTimedout = false;
    return true;
  }
  //return true;
}

char* getHostIPAddress(){
//Determines and returns an IP address of the local host.
  char* ip;
  char buff[1024];
  struct hostent* host;
  gethostname(buff,1024);
  host = gethostbyname(buff);
  char hostbuff[1024];
  gethostname(hostbuff,1024);
  host = gethostbyname(hostbuff);
  ip = inet_ntoa(*((struct in_addr*)host -> h_addr_list[0]));  
  return ip;
}

