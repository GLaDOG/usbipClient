#include "usbipCli.h"

/* 初始化netlink socket,用于listen kernel发送的hotplug消息，定义在handleHotplug.c中 */
int initHotplugSock(void)
{
    struct sockaddr_nl snl;
    int retval;
    const int buffersize = 16 * 1024 * 1024;

    memset(&snl, 0x00, sizeof(struct sockaddr_nl));
    snl.nl_family   = AF_NETLINK;
    snl.nl_pid      = getpid();
    snl.nl_groups   = 1;

    int hotplugSock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (hotplugSock == -1) {
        printf("error getting socket: %s\n", strerror(errno));
        syslog(LOG_DEBUG, "error getting socket: %s\n", strerror(errno));
        return -1;
    }

    /* set receive buffersize */
    setsockopt(hotplugSock, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));
    retval = bind(hotplugSock, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));

    if (retval < 0) {
        printf("bind failed: %s\n", strerror(errno));
        syslog(LOG_DEBUG, "bind failed: %s\n", strerror(errno));
        close(hotplugSock);
        hotplugSock = -1;
        return -1;
    }

    return hotplugSock;
}

char * initBuf()
{
    char *buf = NULL;
    buf = (char *)malloc(UEVENT_BUFFER_SIZE);
    if (buf == NULL) {
        fprintf(stderr, "error buf malloc, %s", strerror(errno));
        syslog(LOG_DEBUG, "error buf malloc, %s", strerror(errno));
        free(buf);
        buf = NULL;
    }
    memset(buf, '\0', UEVENT_BUFFER_SIZE);

    return buf;
}
/* 处理内核消息，剔除冗余，保留设备变更信息，包括action和busid */
busidMsg *getBusid(char *buf)
{
    int i = 0;
    char *buffer = (char *)buf;
    busidMsg *busid = NULL;

    busid = initbusid();

    if (!strncmp(buffer, "add", 3) && strlen(buffer) >= 45) {
        for(i = 0 ; i < 3; i++) {
            *(busid->ubusid + i) = *(buffer + i + 42);
        }
        busid->action = 1;
        runCommand(busid);
        printf("In getbusid:action:%s,busid:%s\n", "add", busid->ubusid);
        syslog(LOG_DEBUG, "In getbusid:action:%s,busid:%s\n", "add", busid->ubusid);
    }
#if 0
    if (!strncmp(buffer, "rem", 3) && strlen(buffer) >= 48) {
        for(i = 0 ; i < 3; i++) {
            *(busid->ubusid + i) = *(buffer + i + 45);
        }
        busid->action = 2;
        runCommand(busid);
        printf("In getBusid:action:%s,busid:%s\n", "remove", busid->ubusid);
    }
#endif
    return busid;
}

/*
 * 接受kernel hotplug msg，并进行相应的处理，同样定义在handleHotplug.c中
 * 这里唯一需要说明的就是：当有USB设备变化时，netlink的套接字会收到一个消息队列
 * 但这消息队列会分成7～8次发送到recv()，所以需要对这些消息进行处理，
 * 对于主要内容相同的消息，只保留一次，其他的过滤掉。
 * 经过观察，同一次设备变动发送的可用的消息，均大于45个字节，且是这45个字节一样的，
 * 并且开头字节都是代表动作的比如"add"代表添加设备，"remove"代表移除设备，在"add"
 * 消息的字节区间[43~45]代表设备id号，"remove"消息的字节区间[46~48]代表设备id号，
 * 这两个key可以提取出来，用于bind操作，并发送给server。
 *
 */
void * handleHotplug(int hotplugSock)
{
    int flag = 0;
    int err;
    int len;
    char *buf = NULL;
    char *bufLTime = NULL;
    char *bufSend = NULL;
    busidMsg *busid = NULL;

    buf = initBuf();
    bufLTime = initBuf();

    while (1) {
        memset(buf, '\0', UEVENT_BUFFER_SIZE);
        err = recv((int)hotplugSock, buf, UEVENT_BUFFER_SIZE, 0);
        if (flag == 0) {
            strncpy(bufLTime, buf, 1024);
            printf("main thread buf:%s\n", buf);
            syslog(LOG_DEBUG, "main thread buf:%s\n", buf);
            busid = getBusid(buf);
            len = send(servfd, busid, sizeof(busidMsg), 0);
            if (len != sizeof(busidMsg)) {
                fprintf(stderr, "error write, %s", strerror(errno));
                syslog(LOG_DEBUG, "error write, %s", strerror(errno));
            }
            ++flag;
        } else {
            if ((strlen(buf) > 40) && !(strncmp(buf, bufLTime, 45))) {
                /* do nothing */
            } else if (strlen(buf) < 40) {
                strcpy(buf, bufLTime);
            } else {
                printf("main thread buf:%s\n", buf);
                syslog(LOG_DEBUG, "main thread buf:%s\n", buf);
                busid = getBusid(buf);
                len = send(servfd, busid, sizeof(busidMsg), 0);
                if (len != sizeof(busidMsg)) {
                    fprintf(stderr, "error write, %s", strerror(errno));
                    syslog(LOG_DEBUG, "error write, %s", strerror(errno));
                }

            }
            strcpy(bufLTime, buf);
        }
    }
    return 0;
}
