/**
 * @file: ftp_server_connection.cpp
 * @author: Anson, 578677684, F20N03, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
 *
 */
#include "../include/ftp_server_connection.hpp"
#include "../include/ftp_server_net_util.hpp"
//#include "../include/ftp_server_connection_listener.hpp"

#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
int sendToRemote(const int sockDescriptor, const char* message, const int messageLength){
//Sends the 'message' of length 'messageLength' to the remote computer.
//Uses a stream socket, represented by 'sockDescriptor', to send the message.
  int s = send(sockDescriptor,message,messageLength,0);
  return s;
}
bool isConnectionReadyToRead(const int sockDescriptor, const int timeoutSec, const int timeoutUSec, bool& isError, bool&isTimedout){
//Returns true if there is any data sent by the remote computer on the stream socket represented by 'sockDescriptor'.
//Waits for the data until 'timeoutSec' + 0.000001x'timeoutUsec' time.
//If no data has been received before the time out, sets 'isTimedout' value to 'true'.
//If any error occurs, sets 'isError' value to 'true'.
//Calls 'isSocketReadyToRead()' function from 'ftp_server_net_util.hpp' to do all of the above.
  return isSocketReadyToRead(sockDescriptor,timeoutSec,timeoutUSec,isError,isTimedout);
}
int receiveFromRemote(const int sockDescriptor, char* message, int messageLength){
//Receives data from the remote computer into a buffer 'message'.
//Sets the length of the received data into 'messageLength'.
//Uses a stream socket, represented by 'sockDescriptor', to receive the data.
  int r = recv(sockDescriptor, message, messageLength,0);
  return r;
}
void closeConnection(int& sockDescriptor){
//Closes the stream socket, represented by 'sockDescriptor'.
  close(sockDescriptor);
}
void closeAllConnections(int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected){
//Closes all the stream sockets, represented by 'controlSockDescriptor', 'dataListenerSockDescriptor', and 'dataSockDescriptor'.
//Sets 'false' to 'isClientConnected'.
  shutdown(controlSockDescriptor,0);
  shutdown(dataListenerSockDescriptor,0);
  shutdown(dataSockDescriptor,0);
  close(controlSockDescriptor);
  close(dataListenerSockDescriptor);
  close(dataSockDescriptor);
  isClientConnected = false;
}
