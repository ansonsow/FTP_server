/**
 * @file: ftp_server_retrieve.cpp
 * @author: Anson su, 578677684, N20F03, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: DEC 2020
 *
 */

#include <string.h>
#include <fstream>
#include <iostream>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


#include "../include/ftp_server_retrieve.hpp"
#include "../include/ftp_server_connection.hpp"
#include "../include/ftp_server_response.hpp"

using namespace std;
int sendFile(char* filename, int& dataSockDescriptor){
//Checks whether the file with the 'filename' is accessible or not.

  if(access(filename,R_OK)==-1){
      //  printf("=================================bad\n");
    return 0;
  }else{
    //If the file is accessible, opens the file and reads and sends the file content chunk by chunk to the client.
    //DATA_SOCKET_SEND_BUFFER_SIZE is used as the chunk size.
    //Sends file chunks using data connection represented by 'dataSockDescriptor'.
    //Returns the total number of bytes sent to the client.


    char buff[DATA_SOCKET_SEND_BUFFER_SIZE];
    int file = open(filename,O_RDONLY);
    int size = read(file,buff,sizeof(buff));
    sendToRemote(dataSockDescriptor,buff,DATA_SOCKET_SEND_BUFFER_SIZE);
    close(file);
    return size;
  }
}
