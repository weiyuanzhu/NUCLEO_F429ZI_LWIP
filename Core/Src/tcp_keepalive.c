#include "tcp_socket_server.h"
#include "tcp_keepalive.h"
#include "socket_wrap.h"
#include "ctype.h"
#include "FreeRTOS.h"
#include "task.h"
static char ReadBuff[BUFF_SIZE];

void vTcpKeepaliveTask(void)
{
    int cfd, n, i, ret;
    struct sockaddr_in server_addr;
    int so_keepalive_val = 1;
    int tcp_keepalive_idle = 3;
    int tcp_keepalive_intvl = 3;
    int tcp_keepalive_cnt = 3;
    int tcp_nodelay = 1;
again:
    cfd = Socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(cfd, SOL_SOCKET, SO_KEEPALIVE, &so_keepalive_val, sizeof(int));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_REMOTE_IP);

    ret = Connect(cfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0)
    {
        // 100ms去连接一次服务器
        vTaskDelay(100);
        goto again;
    }

    //配置心跳检测参数   默认参数时间很长
    setsockopt(cfd, IPPROTO_TCP, TCP_KEEPIDLE, &tcp_keepalive_idle, sizeof(int));
    setsockopt(cfd, IPPROTO_TCP, TCP_KEEPINTVL, &tcp_keepalive_intvl, sizeof(int));
    setsockopt(cfd, IPPROTO_TCP, TCP_KEEPCNT, &tcp_keepalive_cnt, sizeof(int));
    setsockopt(cfd, IPPROTO_TCP, TCP_NODELAY, &tcp_nodelay, sizeof(int));
    printf("server is connect ok\r\n");

    while (1)
    {
        //等待服务器发送数据
        n = Read(cfd, ReadBuff, BUFF_SIZE);
        if (n <= 0)
        {
            goto again;
        }
        //进行大小写转换
        for (i = 0; i < n; i++)
        {
            ReadBuff[i] = toupper(ReadBuff[i]);
        }
        //写回服务器
        n = Write(cfd, ReadBuff, n);
        if (n <= 0)
        {
            goto again;
        }
    }
}
