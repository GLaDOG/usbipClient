#include "usbipCli.h"

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
        return -1;
    }

    /* set receive buffersize */
    setsockopt(hotplugSock, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));
    retval = bind(hotplugSock, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));

    if (retval < 0) {
        printf("bind failed: %s\n", strerror(errno));
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
    }
    memset(buf, '\0', UEVENT_BUFFER_SIZE);

    return buf;
}

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
        printf("action:%s,busid:%s\n", "add", busid->ubusid);
    }

    if (!strncmp(buffer, "rem", 3) && strlen(buffer) >= 48) {
        for(i = 0 ; i < 3; i++) {
            *(busid->ubusid + i) = *(buffer + i + 45);
        }
        busid->action = 1;
        runCommand(busid);
        printf("action:%s,busid:%s\n", "remove", busid->ubusid);
    }

    return busid;
}

void * handleHotplug(void *hotplugSock)
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
    busid = initbusid();

    while (1) {
        memset(buf, '\0', UEVENT_BUFFER_SIZE);
        recv((int)hotplugSock, buf, UEVENT_BUFFER_SIZE, 0);
        if (flag == 0) {
            strncpy(bufLTime, buf, 1024);
            printf("main thread buf:%s\n", buf);
            busid = getBusid(buf);
            len = write(servfd, busid, sizeof(busidMsg));
            if (len != sizeof(busidMsg)) {
                fprintf(stderr, "error write, %s", strerror(errno));
            }
            ++flag;
        } else {
            if ((strlen(buf) > 40) && !(strncmp(buf, bufLTime, 45))) {
                /* do nothing */
            } else if (strlen(buf) < 40) {
                strcpy(buf, bufLTime);
            } else {
                printf("main thread buf:%s\n", buf);
                busid = getBusid(buf);
                len = write(servfd, busid, sizeof(busidMsg));
                if (len) {
                    fprintf(stderr, "error write, %s", strerror(errno));
                }
            }
            strcpy(bufLTime, buf);
        }
    }
    return 0;
}
