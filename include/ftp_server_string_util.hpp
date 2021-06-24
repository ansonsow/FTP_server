/**
 * @file: ftp_server_str_util.hpp
 * @author: Humayun Kabir, Instructor, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 22, 2020
 * 
 */

#ifndef __FTP_SERVER_STR_UTIL_HEADER__
#define __FTP_SERVER_STR_UTIL_HEADER__

void replaceAll(char* str, char find, char replace);
//Replaces all the occurrences of 'find' character in 'str' with 'replace' character.

bool startsWith(const char* str, const char* prefix);
//Returns true if 'str' starts with 'prefix'.

bool contains(const char* str, const char* substr);
//Returns true if 'str' contains 'substr'

void toUpper(char* str);
//Changes all characters of 'str' to upper case.

void toLower(char* str);
//Changes all characters of 'str' to lower case.

void stripLeadingAndTrailingSpaces(char* str);
//Removes all the spaces, if there is any, from the beginning and the ending of 'str'.

void stripNewlineAtEnd(char* str);
//Removes new line character ('\n'), if there is any, from the end of 'str'.

#endif
