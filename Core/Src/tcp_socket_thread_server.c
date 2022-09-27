/* tcp_socket_thread_server.c*/

#include "tcp_socket_server.h"
#include "socket_wrap.h"
#include "ctype.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

char ReadBuff_2[100];


void vNewClientTask(void const *argument)
{
    int cfd = * (int *) argument;
    int n, i;
    for(;;)
    {
        n = Read(cfd, ReadBuff_2, 100);
        if (n <= 0)
        {
            close(cfd);
            vTaskDelete(NULL);
        }
        // lower case to uper
        for(i = 0; i< n; i++) 
        {
            ReadBuff_2[i] = toupper(ReadBuff_2[i]);
        }
        n = Write(cfd, ReadBuff_2, n);
        if(n < 0)
        {
            close(cfd);
            vTaskDelete(NULL);
        }
        n = sprintf(ReadBuff_2, " cfd: %d\r\n", cfd);
        n = Write(cfd,ReadBuff_2,n);
        if(n < 0)
        {
            close(cfd);
            vTaskDelete(NULL);
        }
    }
}


// TCP多线程服务器任务
void vTcpThreadServerTask(void)
{
    int sfd, cfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    BaseType_t ret;
    //创建socket
    sfd = Socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定socket
    Bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    //监听socket
    Listen(sfd, 5);
    //等待客户端连接
    client_addr_len = sizeof(client_addr);

    for(;;) 
    {
        cfd = Accept(sfd, (struct sockaddr *)&client_addr, &client_addr_len);
        printf("client is connect cfd = %d\r\n", cfd);

        ret = xTaskCreate((TaskFunction_t) vNewClientTask,
                                        "Client",
                                        1024,
                                        (void *)&cfd,
                                        osPriorityNormal,
                                        NULL);
        if(ret != pdPASS)
        {
            printf("create task failed\r\n");
        }
    }
    
}
