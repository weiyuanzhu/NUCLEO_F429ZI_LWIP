#include "dns_client.h"
#include "ctype.h"
#include "FreeRTOS.h"
#include "task.h"
#include "netdb.h"

void vDnsClientTask(void){
  int i;
  struct sockaddr_in server_addr;
  //dns 域名解析功能
  struct hostent *p_hostent = NULL;	
  p_hostent = gethostbyname("www.google.com");
  if(p_hostent){
	for(i = 0; p_hostent->h_addr_list[i]; i++){	
	  printf("host ip:%s\r\n", inet_ntoa(*p_hostent->h_addr_list[i]));	
	}
  }else{	
	printf("get host ip fail!\r\n");
  }	
}
