#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

#define MAXLINE 1024
#define IPADDRESS "127.0.0.1"
#define SERV_PORT 8787

#define max(a,b) (a > b) ? a : b

void handle_recv_msg(int sockfd, char *buf);
void handle_connection(int sockfd);

#endif
