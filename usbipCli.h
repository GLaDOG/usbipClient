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
#include <strings.h>
#include <arpa/inet.h>
/* libusbip API */
#include "usbip-utils.h"
#include <syslog.h>

#define MAXLINE 1024
/* server ip address*/
#define IPADDRESS "192.168.6.155"
/* server port */
#define SERV_PORT 25341
/* buffer size */
#define UEVENT_BUFFER_SIZE 2048

/* 与server交流的数据结构 */
typedef struct _busid {
    /* action代表动作"add"或"remove" */
    int  action;
    /* 用于操作usb设备的关键字 */
    char ubusid[4];
}busidMsg;

int servfd;
char *cmd;

/* 初始化cmd，定义在common.c中。最初用于system(),现在并未使用 */
void initCommand();
/* 初始化busid结构，主用于向server发送 */
busidMsg *initbusid();
/* 早期用于执行system(),所以命名为runCommand()，后期更改为使用API，并未做命名修改和结构调整 */
int runCommand(busidMsg *busid);
/* 初始化socket，用于连接server，定义在handleServ.c中 */
void initServerConn(void);
/* 用于处理一些"coldplug"设备，定义在handleLocal.c中 */
int handleLocal(void);
/* listen server socket, receive server's detach msg, 然后由于消息冲突，将其拿掉不再使用 */
void * handleServMsg(void *servfd);
/* 初始化netlink socket,用于listen kernel发送的hotplug消息，定义在handleHotplug.c中 */
int initHotplugSock(void);
/* 接受kernel hotplug msg，并进行相应的处理，同样定义在handleHotplug.c中 */
void * handleHotplug(int hotplugSock);

#endif
