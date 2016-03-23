#include "usbipCli.h"

/*
 * 用于处理一些"coldplug"设备，定义在handleLocal.c中
 * 这里使用的list_devices()函数，并不是原始的list_devices()函数
 * 而是经过修改的，传递的参数是一个字符数组，会在list_devices()中
 * 填充上已存在的USB设备的id，并使用"|"字符分割开来，之后就可以使用
 * strtok()进行处理，分次发送就可以了。另外这里用到的sleep()，
 * 是因为bind()操作不能瞬间完成，需要一定时间执行，否则就会出现识别不了的情况。
 * 目前为止还没有更好的办法。
 *
 */
int handleLocal(void)
{
    int retval = 0;
    busidMsg *lbusid = NULL;
    char devbusid[32];
    char *delim = "|";

    lbusid = initbusid();
    list_devices(devbusid);
    devbusid[strlen(devbusid)-1] = '\0';
    printf("in local devbusid:%s\n", devbusid);
    syslog(LOG_DEBUG, "in local devbusid:%s\n", devbusid);
    char *busid = strtok(devbusid, delim);
    while (busid != NULL) {
        lbusid->action = 1;
        strncpy(lbusid->ubusid, busid, 3);
        runCommand(lbusid);
        retval = send(servfd, lbusid, sizeof(busidMsg), 0);
        if (retval != sizeof(busidMsg)) {
            fprintf(stderr, "error send, %s\n", strerror(errno));
            syslog(LOG_DEBUG, "error send, %s\n", strerror(errno));
        }
        printf("busid:%s\n", busid);
        syslog(LOG_DEBUG, "busid:%s\n", busid);
        busid = strtok(NULL, delim);
        sleep(3);
    }
    free(lbusid);
    lbusid = NULL;
    return;
}
