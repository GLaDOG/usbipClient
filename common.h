#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <asm/types.h>
//该头文件需要放在netlink.h前面防止编译出现__kernel_sa_family未定义
#include <sys/socket.h>
#include <linux/netlink.h>
#include <pthread.h>

#define UEVENT_BUFFER_SIZE 2048

pthread_t ntid;

int init_hotplug_sock(void);
void *getBusid(void *buf);

#endif
