/**
 * @file: ftp_server_session.cpp
 * @author: Anson, 578677684, F20N03, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
 *
 */

#include"../include/ftp_server_session.hpp"
#include"../include/ftp_server_connection.hpp"
#include"../include/ftp_server_request.hpp"
#include"../include/ftp_server_response.hpp"

#include <stdio.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
using namespace std;
void startClientFTPSession(int& controlSockDescriptor){
  bool isClientConnected = true;
  bool isUser = false;
  bool isLoggedIn = false;
  int dataSocketDescriptor = -1;
  int dataListenerSockDescriptor = -1;
  char rootDir[1024];
  bool isError;
  bool isTimeout;
  char* response;
  //Determines the root directory of the FTP server and sets it to the local variable 'rootDir'
//Passes the references of the local variables in various function calls, the called functions then sets
//the appropriate value to these variables.

  getcwd(rootDir, 1024);

  response =strdup(CONNECTED_RESPONSE);
  size_t responseSize = sizeof(char)*strlen(response);
  sendToRemote(controlSockDescriptor,response,responseSize);
  free(response);



  //Waits for client's FTP request for FTP_CLIENT_SESSION_TIMEOUT_SEC + 0.000001xFTP_CLIENT_SESSION_TIMEOUT_USEC time
  //calling 'isConnectionReadyToRead()' function from 'ftp_server_connection.hpp'.
  //If a request comes before the timeout, reads the request.
  //Interprets the request, takes appropriate action, and sends appropriate response to the client by calling
  //'interpreteCommand()' function from 'ftp_server.command.hpp'.


  while (isConnectionReadyToRead(controlSockDescriptor, FTP_CLIENT_SESSION_TIMEOUT_SEC, FTP_CLIENT_SESSION_TIMEOUT_USEC, isError, isTimeout) == true){

    const int cmdSize = 1024;
    char * cmd = new char[cmdSize];
    memset(cmd, 0, cmdSize);
    if(receiveFromRemote(controlSockDescriptor, cmd, cmdSize)!=0){
      interpreteCommand(cmd, controlSockDescriptor, dataListenerSockDescriptor, dataSocketDescriptor, isClientConnected, isUser, isLoggedIn, rootDir);
    }
    delete []cmd;
  }

//closes all connections calling 'closeAllConnections()' function from 'ftp_server_connection.hpp'.
  closeAllConnections(controlSockDescriptor, dataListenerSockDescriptor, dataSocketDescriptor, isClientConnected);

  //If timeout or error happens before a FTP request comes, sends an appropriate response to the client
  //Calls 'sendToRemote()' function from 'ftp_server_connection.hpp' to send response to the client.
  if (isError==true){
    response=strdup(INTERNAL_ERROR_RESPONSE);
    size_t responseSize = sizeof(char)*strlen(response);
    sendToRemote(controlSockDescriptor,response,responseSize);
  }


  if (isTimeout==true){
    response=strdup(PASSIVE_ERROR_TIMEOUT_RESPONSE);
    size_t responseSize = sizeof(char)*strlen(response);
    sendToRemote(controlSockDescriptor,response,responseSize);
  }


  free(response);
}
