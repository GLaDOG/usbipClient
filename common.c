#include "usbipCli.h"

void initCommand()
{
    cmd = (char *)malloc(UEVENT_BUFFER_SIZE);
    if (cmd == NULL) {
        fprintf(stderr, "error cmd malloc, %s", strerror(errno));
    }
    memset(cmd, '\0', UEVENT_BUFFER_SIZE);
}

busidMsg * initbusid()
{
    busidMsg *busid = NULL;
    busid = (busidMsg *)malloc(sizeof(busidMsg));
    if (busid == NULL) {
        fprintf(stderr, "error busid malloc, %s", strerror(errno));
    }
    memset(busid, 0, sizeof(busidMsg));
    return busid;
}

int runCommand(busidMsg *busid)
{
    int ret = -1;
    if (busid->action == 1) {
        ret = bind_device(busid->ubusid);
        sprintf(cmd, "%s%s", "usbip bind --busid=", busid->ubusid);
    } else if (busid->action == 2) {
        ret = unbind_device(busid->ubusid);
        sprintf(cmd, "%s%s", "usbip unbind --busid=", busid->ubusid);
    }
    printf("In runCommand:%s\n", cmd);
    return 1;
}
