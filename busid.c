#include "common.h"

void *
getBusid(void *buf)
{
    int i = 0;
    char *buffer = (char *)buf;
    char busid[7] = {'\0'};

    if (!strncmp(buffer, "add", 3) && strlen(buffer) >= 45) {
        for(i = 0 ; i < 3; i++) {
            busid[i] = buffer[i + 42];
        }
        printf("action:%s,busid:%s\n", "add", busid);
    }

    if (!strncmp(buffer, "rem", 3) && strlen(buffer) >= 48) {
        for(i = 0 ; i < 3; i++) {
            busid[i] = buffer[i + 45];
        }
        printf("action:%s,busid:%s\n", "remove", busid);
    }

    return((void *)0);
}
