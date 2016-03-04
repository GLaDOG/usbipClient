#include "usbipCli.h"

void initServerConn(void)
{
    struct sockaddr_in servaddr;

    servfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,IPADDRESS,&servaddr.sin_addr);

    int retval = 0;
    retval = connect(servfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if (retval < 0) {
        fprintf(stderr, "connect fail,error:%s\n", strerror(errno));
    }
    printf("connect to server\n");
}

void *handleServMsg(void *servfd)
{
    int ret = 0;
    busidMsg *busid;
    busid = initbusid();

    while (1) {
        ret = recv((int)servfd, busid, sizeof(busidMsg), 0);
        printf("In handleServ:action, %d busid, %s\n", busid->action, busid->ubusid);
        if (ret != sizeof(busidMsg)) {
            fprintf(stderr, "error read, %s", strerror(errno));
        }
        runCommand(busid);
    }
}
