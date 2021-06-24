/**
 * @file: ftp_server_nlist.hpp
 * @author: Humayun Kabir, Instructor, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 22, 2020
 * 
 */

#ifndef __FTP_SERVER_NLIST_HEADER__
#define __FTP_SERVER_NLIST_HEADER__

#define DEFAULT_DIRECTORY_PATH "./"

int listDirEntries(int dataSockDescriptor);
//Determines and sends the list of current directory entries to the client using 
//the data connection, represented by 'dataSockDescriptor'.
//Returns the count of entries to the caller.

#endif
