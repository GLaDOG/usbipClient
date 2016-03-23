#include "usbipCli.h"

/* 初始化cmd，定义在common.c中。最初用于system(),现在并未使用 */
void initCommand()
{
    cmd = (char *)malloc(UEVENT_BUFFER_SIZE);
    if (cmd == NULL) {
        fprintf(stderr, "error cmd malloc, %s", strerror(errno));
        syslog(LOG_DEBUG, "error cmd malloc, %s", strerror(errno));
    }
    memset(cmd, '\0', UEVENT_BUFFER_SIZE);
}
/* 初始化busid结构，主用于向server发送 */
busidMsg * initbusid()
{
    busidMsg *busid = NULL;
    busid = (busidMsg *)malloc(sizeof(busidMsg));
    if (busid == NULL) {
        fprintf(stderr, "error busid malloc, %s", strerror(errno));
        syslog(LOG_DEBUG, "error busid malloc, %s", strerror(errno));
    }
    memset(busid, 0, sizeof(busidMsg));
    return busid;
}
/* 早期用于执行system(),所以命名为runCommand()，后期更改为使用API，并未做命名修改和结构调整 */
int runCommand(busidMsg *busid)
{
    int ret = -1;
    if (busid->action == 1) {
        ret = bind_device(busid->ubusid);
        sprintf(cmd, "%s%s", "usbip bind --busid=", busid->ubusid);
        syslog(LOG_DEBUG, "runcommand(), %s", cmd);
    }
#if 0
    else if (busid->action == 2) {
        ret = unbind_device(busid->ubusid);
        sprintf(cmd, "%s%s", "usbip unbind --busid=", busid->ubusid);
    }
#endif
    printf("In runCommand:%s\n", cmd);
    return 1;
}
