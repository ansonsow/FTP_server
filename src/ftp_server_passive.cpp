/**
 * @file: ftp_server_passive.cpp
 * @author: anson su, 578677684, F20N03, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: DEC 2020
 *
 */

#include"../include/ftp_server_passive.hpp"
#include"../include/ftp_server_connection_listener.hpp"
#include"../include/ftp_server_connection.hpp"
#include"../include/ftp_server_net_util.hpp"
#include"../include/ftp_server_request.hpp"
#include"../include/ftp_server_string_util.hpp"
#include"../include/ftp_server_response.hpp"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
using namespace std;


void enteringIntoPassive(const int controlSockDescriptor, int& passiveListenerSockDescriptor, int& dataSockDescriptor){


 bool s = false;
 startPassiveListener(passiveListenerSockDescriptor,s);
//Starts a passive connection listener by calling 'startPassiveListener()' function.

 if(s == true){
   //Once successful on the above function call, sends an appropriate response to the client.
   char r[1024];
   createPassiveSuccessResponse(r,passiveListenerSockDescriptor);
   size_t length = sizeof(char)*strlen(r);
   sendToRemote(controlSockDescriptor,r,length);


   bool error = false;
   bool timeout = false;

   //Waits for a specific time interval to receive a client's connection request on the listener by calling
   //isListenerSocketReady() from 'ftp_server_connection_listener.hpp'.
   bool i = isListenerSocketReady(passiveListenerSockDescriptor, DATA_CONNECTION_TIME_OUT_SEC, DATA_CONNECTION_TIME_OUT_USEC,error,timeout);
   //bool i = true;
   //printf("====================================after i\n");
   if(i==true){
     //Accepts client's connection request, if there is any, and opens a data connection with the client by calling
     //'acceptClientConnetion() function from 'ftp_server_connection_listener.hpp'.
     //printf("====================================i=true\n");
     acceptClientConnection(passiveListenerSockDescriptor,dataSockDescriptor);
     //printf("====================================accepted\n");

     //Closes the connection listener after opening the data connection by calling 'closeListenerSocket()'
     closeListenerSocket(passiveListenerSockDescriptor);
     //printf("====================================closed\n");

     //function from 'ftp_server_connection_listener.hpp'.
     //Sends appropriate response to the client using control connection.
     //Calls 'sendToRemote()' function from 'ftp_server_connection.hpp' to send response to client.
     char* response = strdup(DATA_CONNECTION_SUCCESS_RESPONSE);
     size_t length = sizeof(char)*strlen(response);
     sendToRemote(controlSockDescriptor,response,length);
     free(response);


   }else{
     //printf("====================================i=false\n");
     if(timeout==true){
       char* response = strdup(PASSIVE_ERROR_TIMEOUT_RESPONSE);
       size_t length = sizeof(char)*strlen(response);
       sendToRemote(controlSockDescriptor,response,length);
       free(response);
     }else{
       char* response = strdup(PASSIVE_ERROR_RESPONSE);
       size_t length = sizeof(char)*strlen(response);
       sendToRemote(controlSockDescriptor,response,length);
       free(response);
     }
   }
 }else{
   char* response = strdup(PASSIVE_ERROR_RESPONSE);
   size_t length = sizeof(char)*strlen(response);
   sendToRemote(controlSockDescriptor,response,length);
   free(response);
 }
}
void startPassiveListener(int &listenerSockDescriptor, bool &succeded){
//Starts a passive listener socket that can listen connection requests from the remote computer.
//by calling 'startListenerSocket()' function from 'ftp_server_connection_listener.hpp'.
  char *porta = strdup(PASSIVE_DEFAULT_PORT);
  startListenerSocket(porta, listenerSockDescriptor, succeded);
  free(porta);
}

void createPassiveSuccessResponse(char* response, const int passiveListenerSockDescriptor){
//Creates a Passive success response.
//Determines the passive listener port number from 'passiveListenerSockDescriptor' by calling
//'getPortFromSocketDescriptor()' function from 'ftp_server_net_util.hpp'.
//Determines the local IP address by calling 'getHostIPAddress()' function from 'ftp_server_net_util.hpp'.
//Includes both the IP address and the port number into passive success response according to RFC.
  int port = getPortFromSocketDescriptor(passiveListenerSockDescriptor);
  int p1 = port / 256;
  int p2 = port - p1*256;
  char*add = getHostIPAddress();
  replaceAll(add, '.',',');
  sprintf(response,PASSIVE_SUCCESS_RESPONSE,add,p1,p2);
}
