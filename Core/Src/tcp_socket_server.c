/******socket_tcp_server.c******/
#include "tcp_socket_server.h"
#include "socket_wrap.h"
#include "ctype.h"
char ReadBuff[BUFF_SIZE];
// TCP服务器任务
void vTcpServerTask(void)
{
    int sfd, cfd, n, i;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    //创建socket
    sfd = Socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定socket
    Bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    //监听socket
    printf("Bind and listen on TCP socket\r\n");
    Listen(sfd, 5);
    //等待客户端连接
    client_addr_len = sizeof(client_addr);
again:
    cfd = Accept(sfd, (struct sockaddr *)&client_addr, &client_addr_len);
    printf("client is connect cfd = %d\r\n", cfd);
    while (1)
    {
        //等待客户端发送数据
        n = Read(cfd, ReadBuff, BUFF_SIZE);
        //进行大小写转换
        for (i = 0; i < n; i++)
        {
            ReadBuff[i] = toupper(ReadBuff[i]);
        }
        if((n + 1) < sizeof(ReadBuff))
        {
            ReadBuff[n] = '\r';
            ReadBuff[n + 1] = '\n';
        }

        //写回客户端
        n = Write(cfd, ReadBuff, (n + 2));
        if(n < 0) 
        {
            goto again;
        }
    }
}
