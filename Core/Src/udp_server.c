/******socket_udp_server.c******/
#include "udp_server.h"
#include "tcp_socket_server.h"
#include "socket_wrap.h"
#include "ctype.h"

static char ReadBuff[BUFF_SIZE];

void vUDPServerTask() {
    int sfd, n, i;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    // create server socket
    sfd = Socket(AF_INET, SOCK_DGRAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(UDP_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Bind socket
    Bind(sfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    client_addr_len = sizeof(client_addr);

    while (1)
    {
        n = recvfrom(sfd, ReadBuff, BUFF_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        ReadBuff[n] = '\0';
        printf("udp recv data:%s\r\n", ReadBuff);

        for(i = 0; i < n; i++){
            ReadBuff[i] = toupper(ReadBuff[i]);	
        }	

        sendto(sfd, ReadBuff, n, 0, (struct sockaddr *)&client_addr, client_addr_len);
    }
}