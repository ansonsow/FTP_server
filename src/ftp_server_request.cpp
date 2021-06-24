/**
 * @file: ftp_server_command.cpp
 * @author: Anson, 578677684, N20F03, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: DEC 2020
 *
 */

 #include <stdlib.h>
 #include <string>
 #include <string.h>
 #include <unistd.h>
 #include <signal.h>
 #include <sys/socket.h>
 #include <sys/types.h>
 #include <dirent.h>


#include "../include/ftp_server_request.hpp"
#include "../include/ftp_server_connection.hpp"
#include "../include/ftp_server_passive.hpp"
#include "../include/ftp_server_nlist.hpp"
#include "../include/ftp_server_retrieve.hpp"
#include "../include/ftp_server_response.hpp"
#include "../include/ftp_server_string_util.hpp"
using namespace std;

void interpreteCommand(char* command, int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected, bool& isUser, bool& isLoggedIn, const char* rootDir){
  //initialise variable for command and argument
  char c[COMMAND_CODE_CHARACTER_COUNT];
  char a[COMMAND_ARG_MAX_CHARACTER_COUNT];


//check if the command have a space, if not do the following
  if(contains(command, " ")==false){

    //check if the command is pwd
    if(strncmp(command, COMMAND_PWD, 3) == 0){
      handleCommandPWD(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected, isLoggedIn);
    }

    //check if the command is cdup
    else if(strncmp(command, COMMAND_CDUP, 4) == 0){
      handleCommandCDUP(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected, isLoggedIn, rootDir);
    }

    //check if the command is nlist
    else if(strncmp(command, COMMAND_NLST, 4) == 0){
      handleCommandNLST(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected,isLoggedIn);
    }

    //check if the command is passive
    else if(strncmp(command, COMMAND_PASV, 4) == 0){
      handleCommandPASV(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected,isLoggedIn);
    }

    //check if the command is quit
    else if(strncmp(command, COMMAND_QUIT, 4) == 0){
      handleCommandQUIT(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor,isClientConnected);
    }

    else if(strncmp(command, "Hello!",6)==0){}

    //other commands without space is unsupported
    else{
      handleCommandUnSupported(controlSockDescriptor);
    }

  }else{
    //the command has space
    //seperate the command and argument
    parseCommandLine(command,c,a);


    //check if the command is user
    if(strncmp(c, COMMAND_USER, 4) == 0){
      handleCommandUSER(a, controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected, isUser);
    }

    //check if the command is password
    else if(strncmp(c, COMMAND_PASSWORD, 4) == 0){
      handleCommandPASS(a, controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected, isUser, isLoggedIn);
    }

    //check if the command is cwd
    else if(strncmp(c, COMMAND_CWD, 3) == 0){
      handleCommandCWD(a, controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected, isLoggedIn);
    }

    //check if the command is retrieve
    else if(strncmp(c, COMMAND_RETR, 4) == 0){
      handleCommandRETR(a, controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected, isLoggedIn);
    }

    //other commands with space is unsupported
    else{
      handleCommandUnSupported(controlSockDescriptor);
    }
  }
}


//seperate the command and argument
void parseCommandLine(char* commandLine, char* command, char* argument){

  char *tok = NULL;
  string str[2];
  str[0] = tok = strtok(commandLine," ");
  //what's before the space bar goes into command
  strcpy(command, str[0].c_str());
  str[1] = tok = strtok(NULL, " ");
  //what's after the space bar goes into argument
  strcpy(argument, str[1].c_str());

}

void handleCommandUSER(char* username, int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected, bool& isUser){

  //check if username is equal to default username
  if(strncmp(username, DEFAULT_USERNAME, 7) == 0){
    //if it does set isUser to true
    isUser=true;
    //and send username ok response
    char* response =strdup(USERNAME_OK_RESPONSE);
    size_t responseSize = sizeof(char)*strlen(response);
    sendToRemote(controlSockDescriptor,response,responseSize);
    free(response);
  }else{
    //send invalid username response
    char* response =strdup(INVALID_USERNAME_RESPONSE);
    size_t responseSize = sizeof(char)*strlen(response);
    sendToRemote(controlSockDescriptor,response,responseSize);
    free(response);
    closeAllConnections(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);
  }
}

void handleCommandPASS(char* password, int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected, bool& isUser, bool& isLoggedIn){
  if (isUser==false){
      //It also checks whether USER command was successful or not before this PASS command by checking the value in 'isUser'.
//A 'false' value in 'isUser' means USER command has not been successful before this PASS command.
//If there was no successful USER command, sends an appropriate response to the client and
//closes all connections by calling 'handleNotLoggedIn()' function.

    handleNotLoggedIn(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);
  }else if(strncmp(password, DEFAULT_PASSWORD, 7) != 0){
    //If the password does not match, sends appropriate response to the client and closes all connections
//by calling ''handleNotLoggedIn()' function.

    handleNotLoggedIn(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);
  }else{


//If the password matches, sets 'true' to 'isLoogedIn' and sends appropriate response to the client.
//Calls 'sendToRemote()' function from 'ftp_server_connection.hpp' to send a response to the client.

    isLoggedIn=true;
    char* response =strdup(LOGIN_RESPONSE);
    size_t responseSize = sizeof(char)*strlen(response);
    sendToRemote(controlSockDescriptor,response,responseSize);
    free(response);
  }
}

void handleCommandPWD(int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected, bool& isLoggedIn){
  if (isLoggedIn==false){

//First, checks whether the client is logged in or not by checking the value in 'isLoggedIn'.
//If the client is not logged in, sends an appropriate response to the client and closes all connections
//by calling 'handleNotLoggedIn()' function.

    handleNotLoggedIn(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);
  }else{

//If the client is logged in, determines the current working directory sends it to the client in an appropriate response.
//Calls 'sendToRemote()' function from 'ftp_server_connection.hpp' to send a response to the client.

    char curDir[1024];
    getcwd(curDir, 1024);
    char* response =strdup(curDir);
    size_t responseSize = sizeof(char)*strlen(response);
    sendToRemote(controlSockDescriptor,response,responseSize);
    free(response);
  }
}

void handleCommandCWD(char* directory, int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected, bool& isLoggedIn){
  char curDir[1000];
  getcwd(curDir, 1000);
  string directory_str(directory);
  if(isLoggedIn==false){
//First, checks whether the client is logged in or not by checking the value in 'isLoggedIn'.
//If the client is not logged in, sends an appropriate response to the client and closes all
//connections by calling 'handleNotLoggedIn()' function.
    handleNotLoggedIn(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);
  }else{
    //check if direcotry
    //       is not a subdirectory of the current working directory
    //       starts with "./" or "../"
    //       contains "/.", "/..", or "*"

    if(startsWith(directory, "./")==true||startsWith(directory, "../")==true||contains(directory,"/.")==true||contains(directory,"/..")==true||contains(directory,"*")==true||contains(directory,"${USER}")==true)
    {
      //If the requested directory is not a valid directory, sends an appropriate response to the client.

      char* response =strdup(INVALID_PATH_RESPONSE);
      size_t responseSize = sizeof(char)*strlen(response);
      sendToRemote(controlSockDescriptor,response,responseSize);
      free(response);
    }

    else
    {
      DIR* dir = opendir(directory);
        if (dir){
          //If the requested directory is valid, changes the current directory to the requested directory and sends
          //an appropriate response to the client.

	       closedir(dir);
	       chdir(directory);
	       char* response =strdup(CHANGE_DIRECTORY_RESPONSE);
	       size_t responseSize = sizeof(char)*strlen(response);
	       sendToRemote(controlSockDescriptor,response,responseSize);
	       free(response);

        }else{

          //sends cwd failed response
	       char* response =strdup(CWD_FAIL_RESPONSE);
	       size_t responseSize = sizeof(char)*strlen(response);
	       sendToRemote(controlSockDescriptor,response,responseSize);
	       free(response);

        }
     }
   }
 }


void handleCommandCDUP(int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected, bool& isLoggedIn, const char* rootDir){
  if(isLoggedIn==false){
    //First, checks whether the client is logged in or not by checking the value in 'isLoggedIn'.
//If the client is not logged in, sends an appropriate response to the client and closes all
//connections by calling 'handleNotLoggedIn()' function.

    handleNotLoggedIn(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);
  }else{
    char curDir[1024];
    getcwd(curDir, 1024);
    //check if moving up to the parent directory is falling beyond
    if(strcmp(rootDir,curDir)==0){
      //If the the move goes beyond project's root directory, sends an appropriate response to the client.

      char* response =strdup(CDUP_FAIL_RESPONSE);
      size_t responseSize = sizeof(char)*strlen(response);
      sendToRemote(controlSockDescriptor,response,responseSize);
      free(response);
    }else{
      //If the move does not go beyond project's root directory, changes the current working directory
//to the parent directory and sends an appropriate response to the client.

	chdir("..");
	char* response =strdup(CHANGE_TO_PARENT_DIRECTORY_RESPONSE);
	size_t responseSize = sizeof(char)*strlen(response);
	sendToRemote(controlSockDescriptor,response,responseSize);
	free(response);
      }
  }
}

void handleCommandPASV(int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected, bool& isLoggedIn){
  if(isLoggedIn==false){
    //First, checks whether the client is logged in or not by checking the value in 'isLoggedIn'.
//If the client is not logged in, sends an appropriate response to the client and closes all connections
//by calling 'handleNotLoggedIn()' function.

    handleNotLoggedIn(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);
  }else{
    //If the client is logged in, enters into passive mode by calling 'enteringIntoPassive()' function from 'ftp_server_passive.hpp'
    enteringIntoPassive(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor);
  }
}

void handleCommandNLST(int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected, bool& isLoggedIn){
  if(isLoggedIn==false){
    //First, checks whether the client is logged in or not by checking the value in 'isLoggedIn'.
//If the client is not logged in, sends an appropriate response to the client and closes all connections
//by calling 'handleNotLoggedIn()' function.

    handleNotLoggedIn(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);
  }
//it checks whether the data connection has already been established by
//a PASV command before this NLST command.
  else if(dataSockDescriptor < 0){

    char* response =strdup(DATA_OPEN_CONNECTION_ERROR_RESPONSE);
    size_t responseSize = sizeof(char)*strlen(response);
    sendToRemote(controlSockDescriptor,response,responseSize);
    free(response);

  }else{
//calling 'listDirEntries()' function
//from 'ftp_server_nlist.hpp'.
    char response[1024];
    int l=listDirEntries(dataSockDescriptor);
    sprintf(response,NLST_CONNECTION_CLOSE_RESPONSE,l);
    sendToRemote(controlSockDescriptor,response,1024);
    close(dataSockDescriptor);

  }
}

void handleCommandRETR(char* argument, int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected, bool& isLoggedIn){

  if(isLoggedIn==false){
    //checks whether the client is logged in or not by checking the value in 'isLoggedIn'.
//If the client is not logged in, sends an appropriate response to the client and closes all
//connections by calling 'handleNotLoggedIn()' function.

    handleNotLoggedIn(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);
  }
  else if(dataSockDescriptor < 0){
//it checks whether the data connection has already been established by a PASV
    char* response =strdup(DATA_OPEN_CONNECTION_ERROR_RESPONSE);
    size_t responseSize = sizeof(char)*strlen(response);
    sendToRemote(controlSockDescriptor,response,responseSize);
    free(response);

  }else{
    //calling 'sendFile()' function
    int s = sendFile(argument,dataSockDescriptor);

    if(s==0){

      char* response =strdup(RETR_UNAVAILABLE_ERROR_RESPONSE);
    	size_t responseSize = sizeof(char)*strlen(response);
    	sendToRemote(controlSockDescriptor,response,responseSize);
    	free(response);
    }else{
      //Sends the count of the bytes sent in an appropriate response message to the client using the control connection.

      char response[1024];

      for(int i=0;i<=1024;i++){
        response[i]='\0';
      }

      sprintf(response,RETR_CONNECTION_CLOSE_RESPONSE,s);
      size_t responseSize = sizeof(char)*strlen(response);
      sendToRemote(controlSockDescriptor,response,responseSize);

    }
  }
}


void handleCommandQUIT(int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor,bool& isClientConnected){
  //Closes all connections by calling 'closeAllConnections()' function from 'ftp_server.connection.hpp'.
  //Calls 'sendToRemote()' function from 'ftp_server_connection.hpp' to send a response to the client.

  char* response =strdup(QUIT_RESPONSE);
  size_t responseSize = sizeof(char)*strlen(response);
  sendToRemote(controlSockDescriptor,response,responseSize);
  free(response);
  closeAllConnections(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);

}


void handleCommandUnSupported(const int controlSockDescriptor){

  //Sends an appropriate response saying the requested command is not supported by this FTP server.

  char* response =strdup(UNSUPPORTED_COMMAND_RESPONSE);
  size_t responseSize = sizeof(char)*strlen(response);
  sendToRemote(controlSockDescriptor, response, responseSize);

}


void handleNotLoggedIn(int& controlSockDescriptor, int& dataListenerSockDescriptor, int& dataSockDescriptor, bool& isClientConnected){
  //Sends an appropriate response saying the user is not logged in.
  //Closes all connections by calling 'closeAllConnections()' function from 'ftp_server.connection.hpp'.
  //Calls 'sendToRemote()' function from 'ftp_server_connection.hpp' to send a response to the client.

  char* response =strdup(NOT_LOGGED_IN_RESPONSE);
  size_t responseSize = sizeof(char)*strlen(response);
  sendToRemote(controlSockDescriptor,response,responseSize);
  free(response);
  closeAllConnections(controlSockDescriptor, dataListenerSockDescriptor, dataSockDescriptor, isClientConnected);

}
