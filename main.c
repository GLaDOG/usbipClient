#include "usbipCli.h"

int main(int argc, char *argv[])
{
    int err = 0;
    pthread_t tid1, tid2;
    void *tret;
    /* 日志记录 */
    openlog("USBIP-CLIENT", LOG_CONS | LOG_PID, 0);
    /* 初始化cmd，定义在common.c中。最初用于system(),现在并未使用 */
    initCommand();
    /* 初始化socket，用于连接server，定义在handleServ.c中 */
    initServerConn();
    if (servfd == 0) {
        fprintf(stderr, "error servfd init, %s", strerror(errno));
        syslog(LOG_DEBUG, "error servfd init, %s", strerror(errno));
        return -1;
    }
    /* 用于处理一些"coldplug"设备，定义在handleLocal.c中 */
    handleLocal();
#if 0
    err = pthread_create(&tid1, NULL, handleServMsg, (void *)servfd);
    if (err != 0) {
        fprintf(stderr, "error create pthread1, %s", strerror(errno));
        return -1;
    }
#endif
    int hotplugSock = 0;
    /* 初始化netlink socket,用于listen kernel发送的hotplug消息，定义在handleHotplug.c中 */
    hotplugSock = initHotplugSock();
    if (hotplugSock == 0) {
        fprintf(stderr, "error init hotplug, %s", strerror(errno));
        syslog(LOG_DEBUG, "error init hotplug, %s", strerror(errno));
        return -1;
    }
    /* 接受kernel hotplug msg，并进行相应的处理，同样定义在handleHotplug.c中 */
    handleHotplug(hotplugSock);
#if 0
    err = pthread_create(&tid2, NULL, handleHotplug, (void *)hotplugSock);
    if (err != 0) {
        fprintf(stderr, "error create pthread2, %s", strerror(errno));
        return -1;
    }

    err = pthread_join(tid1, &tret);
    if (err != 0) {
        fprintf(stderr, "error pthread1 join, %s", strerror(errno));
        return -1;
    }
    err = pthread_join(tid2, &tret);
    if (err != 0) {
        fprintf(stderr, "error pthread2 join, %s", strerror(errno));
        return -1;
    }
#endif
    return 0;
}
