#include "usbipCli.h"

/* 初始化socket，用于连接server，定义在handleServ.c中 */
void initServerConn(void)
{
    struct sockaddr_in servaddr;

    servfd = socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET,IPADDRESS,&servaddr.sin_addr);

    int retval = 0;
    while (1) {
        retval = connect(servfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
        if (retval < 0) {
            if (errno == 111) {
                fprintf(stderr, "connect failed,error:%s\n", strerror(errno));
                syslog(LOG_DEBUG, "connect failed,error:%s\n", strerror(errno));
                sleep(2);
                continue;
            }
            close(servfd);
            free(cmd);
            cmd = NULL;
            exit(-1);
        }
        break;
    }
    printf("connect to server\n");
    syslog(LOG_DEBUG, "connect to server\n");
}
/* listen server socket, receive server's detach msg, 然后由于消息冲突，将其拿掉不再使用 */
void *handleServMsg(void *servfd)
{
    int ret = 0;
    busidMsg *busid;
    busid = initbusid();

    while (1) {
        ret = recv((int)servfd, busid, sizeof(busidMsg), 0);
        if (ret <= 0) {
            fprintf(stderr, "client:server is closed.\n");
            syslog(LOG_DEBUG, "client:server is closed.\n");
            close((int)servfd);
            free(cmd);
            cmd = NULL;
            free(busid);
            busid = NULL;
            exit(-1);
        }
        printf("In handleServ:action, %d busid, %s\n", busid->action, busid->ubusid);
        syslog(LOG_DEBUG, "In handleServ:action, %d busid, %s\n", busid->action, busid->ubusid);
        runCommand(busid);
    }
}
