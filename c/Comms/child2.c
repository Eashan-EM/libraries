#include <stdio.h>
#include "Comms.h"

int callback2(unsigned char to, unsigned char from, unsigned short func, unsigned int value) {
  printf("In Child1=> TO: %d, FROM: %d, FUNC: %d, VALUE: %d\n", to, from, func, value);
}

int main() {
  comms_connect();
  comms_subscribe_from(3, callback2);
  int out;
  printf("Read?(Type 0 to exit): ");
  scanf("%d", &out);
  while (out!=0) {
    comms_read();
    printf("Read?(Type 0 to exit): ");
    scanf("%d", &out);
  }
  comms_close();
}