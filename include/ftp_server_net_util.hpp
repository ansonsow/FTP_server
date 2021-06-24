/**
 * @file: ftp_server_net_util.hpp
 * @author: Humayun Kabir, Instructor, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 22, 2020
 * 
 */

#ifndef __FTP_SERVER_NET_UTIL_HEADER__
#define __FTP_SERVER_NET_UTIL_HEADER__

#define MAX_IP_ADDRESS_LENGTH 1024

void closeSocket(int& sockDescriptor);
//Closes the stream socket, represented by 'sockDescriptor'.

int getPortFromSocketDescriptor(const int sockDescriptor);
//Determines and returns the associated port number from a given socket descriptor.

bool isSocketReadyToRead(const int sockDescriptor, const int timeoutSec, const int timeoutUSec, bool& isError, bool& isTimedout); 
//Returns true if there is any data sent by the remote computer on the stream socket represented by 'sockDescriptor'.
//Waits for the data until 'timeoutSec' + 0.000001x'timeoutUsec' time.
//If no data has been sent before the time out, sets 'isTimedout' value to 'true'.
//If any error occurs, sets 'isError' value to 'true'.

char* getHostIPAddress();
//Determines and returns an IP address of the local host.

#endif
