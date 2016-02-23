#ifndef HOTPLUG_H
#define HOTPLUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <asm/types.h>
#include <sys/socket.h>
#include <linux/netlink.h>

#define UEVENT_BUFFER_SIZE 2048

int init_hotplug_sock(void);

#endif
