/**
 * @file: ftp_server_retrieve.hpp
 * @author: Humayun Kabir, Instructor, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 22, 2020
 * 
 */

#ifndef __FTP_SERVER_RETRIEVE_HEADER__
#define __FTP_SERVER_RETRIEVE_HEADER__

#define FILE_OPEN_MODE "r"
#define DATA_SOCKET_SEND_BUFFER_SIZE 65536 //64K

int sendFile(char* filename, int& dataSockDescriptor);
//Checks whether the file with the 'filename' is accessible or not.
//If the file is accessible, opens the file and reads and sends the file content chunk by chunk to the client.
//DATA_SOCKET_SEND_BUFFER_SIZE is used as the chunk size.
//Sends file chunks using data connection represented by 'dataSockDescriptor'.
//Returns the total number of bytes sent to the client.

#endif
