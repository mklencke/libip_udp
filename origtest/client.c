#include <stdio.h>
#include "inet.h"
#include "ip.h"

#define MINIXT1_1 "192.168.0.1"
#define MINIXT1_2 "192.168.0.2"

int main(void) {
  int len;
  len = ip_send(inet_aton(MINIXT1_2),IP_PROTO_UDP,2,"foo bar",8);
  printf("len=%d\n",len);
}

