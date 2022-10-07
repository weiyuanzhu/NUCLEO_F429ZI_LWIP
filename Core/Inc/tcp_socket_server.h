/******socket_tcp_server.h******/
#ifndef _TCP_SOCKET_SERVER_H
#define _TCP_SOCKET_SERVER_H
#define SERVER_LOCAL_IP	"192.168.1.17" //客户端编程时需要用到该服务器地址
#define SERVER_REMOTE_IP	"192.168.0.82" // Remote Server IP
#define SERVER_PORT	  6666
#define BUFF_SIZE	  1024

void vTcpServerTask(void);

#endif 