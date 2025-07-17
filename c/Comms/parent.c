#include <stdio.h>
#include "Comms.h"

int main() {
  comms_open();
  unsigned char to, from, func, value;
  int out;
  printf("Send Another Message?(Type 0 to exit): ");
  scanf("%d", &out);
  while (out!=0) {
    printf("TO: ");
    scanf("%d", &to);
    printf("FROM: ");
    scanf("%d", &from);
    printf("FUNC: ");
    scanf("%d", &func);
    printf("VALUE: ");
    scanf("%d", &value);
    comms_send(to, from, func, value);
    printf("Message Sent!\nSend Another Message?(Type 0 to exit): ");
    scanf("%d", &out);
  }
  comms_close();
}