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
    if (busid->action == 1) {
        sprintf(cmd, "%s %s", "usbip bind --busid=", busid->ubusid);
        system(cmd);
    } else if (busid->action == 2) {
        sprintf(cmd, "%s %s", "usbip unbind --busid=", busid->ubusid);
        system(cmd);
    }
}
