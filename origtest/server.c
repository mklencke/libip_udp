#include <stdio.h>
#include <stdlib.h>
#include "inet.h"
#include "ip.h"

int main(void) {
  int len;
  ipaddr_t from, dst;
  unsigned short proto, idp;
  char *data;
  
  len = ip_receive(&from,&dst,&proto,&idp,&data);
  printf("Received packet: %s (len=%d)\n",data,len);
  free(data);
}

