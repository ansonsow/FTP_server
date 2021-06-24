/**
 * @file: ftp_server_nlist.cpp
 * @author: Anson, 578677684, F20N03, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
 *
 */
#include <dirent.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include"../include/ftp_server_nlist.hpp"
#include"../include/ftp_server_response.hpp"
int listDirEntries(int dataSockDescriptor){
//Determines and sends the list of current directory entries to the client using
//the data connection, represented by 'dataSockDescriptor'.
//Returns the count of entries to the caller
  DIR *dirp = NULL;
  struct stat buff;
  struct dirent *fname;
  int count = 0;
  if((dirp = opendir(DEFAULT_DIRECTORY_PATH))!=NULL){
    while((fname = readdir(dirp))){
      if(DT_REG == fname ->d_type){
        stat(fname->d_name, &buff);
        dprintf(dataSockDescriptor,"file=======================%s     %ld\n",fname->d_name,buff.st_size);
      }
      if(DT_DIR == fname->d_type){
        dprintf(dataSockDescriptor,"directory=============%s\n",fname->d_name);
      }
      count++;
    }
    closedir(dirp);
  }else{
    return 0;
  }
  return count;
}
