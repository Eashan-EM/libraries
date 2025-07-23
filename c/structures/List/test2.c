#include <stdio.h>
#include "List.h"

int main() {
  List *list = list_new();
  for (int i=0; i<10; i++)
    list_add_value_to_tail(list, (void *)i, NULL);
  list_goto_head(list);
  int v;
  while (list_has_next(list)) {
    v = (int)list_get_value(list); 
    list_goto_next(list);
    printf("%d\n", v);
  }
}
