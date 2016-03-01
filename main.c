#include "usbipCli.h"

int main(int argc, char *argv[])
{
    int err = 0;
    pthread_t tid1, tid2;
    void *tret;

    initCommand();

    initServerConn();
    if (servfd == 0) {
        fprintf(stderr, "error servfd init, %s", strerror(errno));
        return -1;
    }
    err = pthread_create(&tid1, NULL, handleServMsg, (void *)servfd);
    if (err != 0) {
        fprintf(stderr, "error create pthread1, %s", strerror(errno));
        return -1;
    }

    int hotplugSock = 0;

    hotplugSock = initHotplugSock();
    if (hotplugSock == 0) {
        fprintf(stderr, "error init hotplug, %s", strerror(errno));
        return -1;
    }
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

    return 0;
}
