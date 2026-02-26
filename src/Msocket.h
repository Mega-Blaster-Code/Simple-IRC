#ifndef __M_SOCKET_H
#define __M_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include "safema.h"

#define M_IPV4 AF_INET
#define M_UDP SOCK_DGRAM
#define M_TCP SOCK_STREAM

#define FAIL_CREATE_SERVER_SOCKET -1
#define FAIL_CREATE_CLIENT_SOCKET -2
#define FAIL_SETSOCKOPT -3
#define FAIL_INET_PTON -4
#define FAIL_BIND -5
#define FAIL_LISTEN -6
#define FAIL_LISTEN -6

typedef struct Mclient {
	int connfd;
	struct sockaddr_in client_socket;
	socklen_t client_len;
} Mclient;

typedef struct Mserver {
	int connfd;
	struct sockaddr_in client_socket;
	socklen_t client_len;
} Mserver;

typedef struct {
	int version;
	int method;
	const char *body;
	size_t body_len;
} Mrequest;

typedef struct Mmessage{
	ssize_t data_len;
	char *data;
}Mmessage;
#include "Msocket.h"

int McreateServerSocket(int domain, int type, int protocol);

int McreateServerAddress(const char *ip_address, int port, struct sockaddr_in *address);

int McreateClientSocket(int domain, int type, int protocol, int *fd);

int Mbind(int fd, struct sockaddr_in *addr);
int newServer(const char *ip, int port);

int newClient(int *socketfd);

int MclientConnect(int socketfd, const char *ip_address, int port);

Mclient MgetServerNewClient(int fd);

int Mseek(int fd, int timeout_ms);

int Mreceive(int fd, Mmessage *msg);

int MreceiveAll(int fd, Mmessage *msg);

int Msend(int fd, Mmessage *msg);

#endif