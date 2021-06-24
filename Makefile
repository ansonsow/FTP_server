#Makefile for CSCI 460 project2 

CC = g++
WFLAG = -Wall  		#Warning Flag
DFLAG = -ggdb		#Debugging Flag
CFLAG = -c		#Compiling Flag
SOURCES = src
INCLUDES = include
BUILDS = build
BINARY = bin
TESTS = test

all: ${BINARY}/ftpserver  ${TESTS}/${BINARY}/ftpservertest

${BINARY}/ftpserver: ${BUILDS}/ftp_server.o ${BUILDS}/ftp_server_connection_listener.o \
		${BUILDS}/ftp_server_net_util.o	${BUILDS}/ftp_server_string_util.o ${BUILDS}/ftp_server_connection.o \
		${BUILDS}/ftp_server_session.o ${BUILDS}/ftp_server_request.o ${BUILDS}/ftp_server_passive.o \
		${BUILDS}/ftp_server_nlist.o ${BUILDS}/ftp_server_retrieve.o
	${CC} ${WFLAG} ${DFLAG} -o ${BINARY}/ftpserver ${BUILDS}/ftp_server.o ${BUILDS}/ftp_server_connection_listener.o \
	${BUILDS}/ftp_server_net_util.o	${BUILDS}/ftp_server_string_util.o ${BUILDS}/ftp_server_connection.o \
	${BUILDS}/ftp_server_session.o ${BUILDS}/ftp_server_request.o ${BUILDS}/ftp_server_passive.o \
	${BUILDS}/ftp_server_nlist.o ${BUILDS}/ftp_server_retrieve.o


${TESTS}/${BINARY}/ftpservertest: ${TESTS}/${BUILDS}/ftp_server_test.o ${TESTS}/${BUILDS}/ftp_server_test_net_util.o \
		${BUILDS}/ftp_server_string_util.o \
		${BUILDS}/ftp_server_net_util.o \
		${BUILDS}/ftp_server_connection_listener.o \
		${BUILDS}/ftp_server_connection.o \
		${BUILDS}/ftp_server_passive.o \
		${TESTS}/${BUILDS}/ftp_client_connection.o \
		${TESTS}/${BUILDS}/ftp_client_control.o \
		${TESTS}/${BUILDS}/ftp_client_command.o
	${CC} ${WFLAG} -lboost_unit_test_framework -o ${TESTS}/${BINARY}/ftpservertest \
	${BUILDS}/ftp_server_string_util.o \
	${BUILDS}/ftp_server_net_util.o \
	${BUILDS}/ftp_server_connection_listener.o \
	${BUILDS}/ftp_server_connection.o \
	${BUILDS}/ftp_server_passive.o \
	${TESTS}/${BUILDS}/ftp_server_test.o \
	${TESTS}/${BUILDS}/ftp_server_test_net_util.o \
	${TESTS}/${BUILDS}/ftp_client_connection.o \
	${TESTS}/${BUILDS}/ftp_client_control.o \
	${TESTS}/${BUILDS}/ftp_client_command.o


${BUILDS}/ftp_server.o: ${SOURCES}/ftp_server.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server.o ${SOURCES}/ftp_server.cpp

${BUILDS}/ftp_server_connection_listener.o: ${SOURCES}/ftp_server_connection_listener.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server_connection_listener.o \
	${SOURCES}/ftp_server_connection_listener.cpp

${BUILDS}/ftp_server_connection.o: ${SOURCES}/ftp_server_connection.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server_connection.o ${SOURCES}/ftp_server_connection.cpp

${BUILDS}/ftp_server_session.o: ${SOURCES}/ftp_server_session.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server_session.o ${SOURCES}/ftp_server_session.cpp

${BUILDS}/ftp_server_net_util.o: ${SOURCES}/ftp_server_net_util.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server_net_util.o ${SOURCES}/ftp_server_net_util.cpp

${BUILDS}/ftp_server_string_util.o: ${SOURCES}/ftp_server_string_util.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server_string_util.o ${SOURCES}/ftp_server_string_util.cpp

${BUILDS}/ftp_server_request.o: ${SOURCES}/ftp_server_request.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server_request.o ${SOURCES}/ftp_server_request.cpp

${BUILDS}/ftp_server_passive.o: ${SOURCES}/ftp_server_passive.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server_passive.o ${SOURCES}/ftp_server_passive.cpp

${BUILDS}/ftp_server_nlist.o: ${SOURCES}/ftp_server_nlist.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server_nlist.o ${SOURCES}/ftp_server_nlist.cpp

${BUILDS}/ftp_server_retrieve.o: ${SOURCES}/ftp_server_retrieve.cpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_server_retrieve.o ${SOURCES}/ftp_server_retrieve.cpp

##
##${TESTS}/${BUILDS}/ftp_server_test.o: ${TESTS}/${SOURCES}/ftp_server_test.cpp ${INCLUDES}/ftp_server_response.hpp \
##		${INCLUDES}/ftp_server_string_util.hpp ${INCLUDES}/ftp_server_net_util.hpp \
##		${INCLUDES}/ftp_server_connection_listener.hpp ${INCLUDES}/ftp_server_connection.hpp \
##		${INCLUDES}/ftp_server_passive.hpp \
##		${TESTS}/${INCLUDES}/ftp_client_connection.hpp ${TESTS}/${INCLUDES}/ftp_client_control.hpp \
##		${TESTS}/${INCLUDES}/ftp_client_command.hpp
##	${CC} ${WFLAG} ${CFLAG} -I${INCLUDES} -I${TESTS}/${INCLUDES} -o ${TESTS}/${BUILDS}/ftp_server_test.o \
##		${TESTS}/${SOURCES}/ftp_server_test.cpp
##
##${TESTS}/${BUILDS}/ftp_server_test_net_util.o: ${TESTS}/${SOURCES}/ftp_server_test_net_util.cpp \
##		${TESTS}/${INCLUDES}/ftp_server_test_net_util.hpp ${INCLUDES}/ftp_server_response.hpp
##	${CC} ${WFLAG} ${CFLAG} -I${INCLUDES} -I${TESTS}/${INCLUDES} -o ${TESTS}/${BUILDS}/ftp_server_test_net_util.o \
##		${TESTS}/${SOURCES}/ftp_server_test_net_util.cpp
##

.PHONY: clean run runclient test install uninstall

clean:
	rm -rf ${BINARY}/*
	rm -f ${TESTS}/${BINARY}/ftpservertest
	rm -rf ${BUILDS}/*

run:	${BINARY}/ftpserver
	${BINARY}/ftpserver 2020

runclient: ${TESTS}/${BINARY}/ftpclient
	${TESTS}/${BINARY}/ftpclient `hostname -I` 2020

test:   ${BINARY}/ftpserver ${TESTS}/${BINARY}/ftpservertest
	${TESTS}/${BINARY}/ftpservertest --log_level=all

install: ${BINARY}/ftpserver
	@echo Sorry, installation has not been done.

uninstall:
	@echo Sorry, uninstallation has not been done.




