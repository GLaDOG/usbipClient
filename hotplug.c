#include "hotplug.h"

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
