/**
 * @file: ftp_server_connection_listener.cpp
 * @author: Anson, 578677684, F20N03, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
 *
 */
#include"../include/ftp_server_connection_listener.hpp"
#include"../include/ftp_server_net_util.hpp"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
void startListenerSocket(char* port, int& listenerSockDescriptor, bool& succeded){
//Starts a listener socket that can listen connection requests from the remote computer.
//Creates a stream socket and binds it to a local IP address and to the given port number.
//Once the binding is successful listens for connection request from the remote computers.
  char host[1024];
  gethostname(host,1024);
  struct addrinfo h;
  memset(&h,0,sizeof(struct addrinfo));
  h.ai_flags = AI_PASSIVE;
  h.ai_family = AF_UNSPEC;
  h.ai_socktype = SOCK_STREAM;
  struct addrinfo* r;
  getaddrinfo(host,port,&h,&r);
  //  char* ip=NULL;
  char lport[10];
  struct addrinfo* result = r;
  while(result!= NULL){
    listenerSockDescriptor = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
    if(listenerSockDescriptor!= -1){
      int opt = 1;
      if(setsockopt(listenerSockDescriptor,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int))==0){
	if(bind(listenerSockDescriptor,result->ai_addr,result->ai_addrlen) == 0){
	  struct sockaddr_in* addrinfo_Address = (struct sockaddr_in*)result->ai_addr;
	  //char* ip =
	  inet_ntoa(addrinfo_Address->sin_addr);
	  sprintf(lport,"%d",ntohs(addrinfo_Address->sin_port));
	  break;
	}
      }
      close(listenerSockDescriptor);
      listenerSockDescriptor = -1;
    }
    result=result->ai_next;
  }
  if(listen(listenerSockDescriptor,10)==0){
    succeded=true;
  }else{
    succeded = false;
  }
}

bool isListenerSocketReady(const int sockDescriptor, const int timeoutSec, const int timeoutUSec, bool& isError, bool&isTimedout){
//Returns true if there is any remote connection request on the listener socket represented by 'sockDescriptor'.
//Waits for a connection request until 'timeoutSec' + 'timeoutUsec' time.
//If no connection request has been received before the time out, sets 'isTimedout' value to 'true'.
//If any error occurs, sets 'isError' value to 'true'.
//Calls 'isSocketReadyToRead()' function from 'ftp_server_net_util.hpp' to do all of the above.
  return isSocketReadyToRead(sockDescriptor,timeoutSec,timeoutUSec,isError,isTimedout);
}

void acceptClientConnection(const int listenerSockDescriptor, int& clientSockDescriptor){
//Accepts a connection request on the listener socket represented by 'listenerSockDescriptor'.
//Opens the connection to the remote computer and represents the connection by 'clientSockDescriptor'.
  struct sockaddr_in addr;
  socklen_t len = sizeof addr;
  memset(&addr, 0 , len);
  // clientSockDescriptor = -1;
  //clientSockDescriptor = accept(listenerSockDescriptor, (struct sockaddr*)&addr, &len);
  int i = accept(listenerSockDescriptor, (struct sockaddr*)&addr, &len);
  if(i==-1){

  }else{
    clientSockDescriptor = i;
  }
}

void closeListenerSocket(int& listenerSockDescriptor){
//Closes the listener socket represented by 'listenerSockDescriptor'.
  closeSocket(listenerSockDescriptor);
}
