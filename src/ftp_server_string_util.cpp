/**
 * @file: ftp_server_string_util.cpp
 * @author: Anson Su 578677684 F20N03, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
 *
 */

#include "../include/ftp_server_string_util.hpp"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
using namespace std;
void replaceAll(char* str, char find, char replace){
//Replaces all the occurrences of 'find' character in 'str' with 'replace' character.
  for(int i=0;str[i]!='\0';i++){
    if (str[i]==find){
      str[i]=replace;
    }else
      continue;
  }
}

bool startsWith(const char* str, const char* prefix){
//Returns true if 'str' starts with 'prefix'.
  int prelen = strlen(prefix);
  for(int i =0;i<prelen;i++){
    if(str[i]!=prefix[i]){
      return false;
    }
  }
  return true;
}
bool contains(const char* str, const char* substr){
//Returns true if 'str' contains 'substr'
  if(strstr(str,substr) != NULL){
    return true;
  }
  return false;
}

void toUpper(char* str){
//Changes all characters of 'str' to upper case.
  int i =0;
  while(str[i]){
    str[i]=toupper(str[i]);
    i++;
  }
}

void toLower(char* str){
//Changes all characters of 'str' to lower case.
  int i =0;
  while(str[i]){
    str[i]=tolower(str[i]);
    i++;
  }
}

void stripLeadingAndTrailingSpaces(char* str){
//Removes all the spaces, if there is any, from the beginning and the ending of 'str'.
  char*copy = str;
  int strl = strlen(str);
  while(copy[strl-1]==' '){
    copy[--strl]=0;
  }
  while(*copy && (*copy)==' '){
    ++copy;
    --strl;
  }
  memmove(str,copy,strl+1);
  //  free copy;
}

void stripNewlineAtEnd(char* str){
//Removes new line character ('\n'), if there is any, from the end of 'str'. 
  int strl = strlen(str) - 1;
  while(str[strl]=='\n'){
    str[strl]='\0';
    strl--;
  }
}
