#include "common.h"

int init_hotplug_sock(void)
{
    struct sockaddr_nl snl;
    int retval;
    const int buffersize = 16 * 1024 * 1024;

    memset(&snl, 0x00, sizeof(struct sockaddr_nl));
    snl.nl_family   = AF_NETLINK;
    snl.nl_pid      = getpid();
    snl.nl_groups   = 1;

    int hotplug_sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_KOBJECT_UEVENT);
    if (hotplug_sock == -1) {
        printf("error getting socket: %s\n", strerror(errno));
        return -1;
    }

    /* set receive buffersize */
    setsockopt(hotplug_sock, SOL_SOCKET, SO_RCVBUFFORCE, &buffersize, sizeof(buffersize));
    retval = bind(hotplug_sock, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));

    if (retval < 0) {
        printf("bind failed: %s\n", strerror(errno));
        close(hotplug_sock);
        hotplug_sock = -1;
        return -1;
    }

    return hotplug_sock;
}

int main(int argc, char *argv[])
{
    int flag = 0;
    int err;
    int hotplug_sock = init_hotplug_sock();
    char buf[UEVENT_BUFFER_SIZE * 2] = {0};
    char bufLTime[UEVENT_BUFFER_SIZE * 2] = {0};
    memset(bufLTime, '\0', UEVENT_BUFFER_SIZE * 2);

    while (1) {
        memset(buf, '\0', UEVENT_BUFFER_SIZE * 2);
        recv(hotplug_sock, buf, sizeof(buf), 0);
        if (flag == 0) {
            strncpy(bufLTime, buf, 1024);
            printf("main thread buf:%s\n", buf);
            char data[4096]={'\0'};
            strcpy (data, buf);
            err=pthread_create(&ntid, NULL, getBusid, (void *)data);
            ++flag;
        } else {
            if ((strlen(buf) > 40) && !(strncmp(buf, bufLTime, 45))) {
                /* do nothing */
            } else if (strlen(buf) < 40) {
                strcpy(buf, bufLTime);
            } else {
                printf("main thread buf:%s\n", buf);
                char data[4096]={'\0'};
                strcpy (data, buf);
                err=pthread_create(&ntid, NULL, getBusid, (void *)data);
            }
            strcpy(bufLTime, buf);
        }
    }
    return 0;
}
