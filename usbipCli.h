#ifndef USBIPCLI_H
#define USBIPCLI_H

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
#include <asm/types.h>
#include <linux/netlink.h>
#include <pthread.h>

#define MAXLINE 1024
#define IPADDRESS "192.168.6.164"
#define SERV_PORT 25341
#define UEVENT_BUFFER_SIZE 2048

typedef struct _busid {
    int  action;
    char ubusid[4];
}busidMsg;

int servfd;
char *cmd;

void initCommand();
busidMsg *initbusid();
int runCommand(busidMsg *busid);
int bind_device(char *busid);
int unbind_device(char *busid);
void initServerConn(void);
void * handleServMsg(void *servfd);
int initHotplugSock(void);
void * handleHotplug(void *hotplugSock);

#endif
