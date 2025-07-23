#include <stdio.h>
#include "List.h"

int main() {
  List *list = list_new();
  list_goto_head(list);
  list_goto_tail(list);
  list_goto_next(list);
  list_goto_prev(list);
  printf("Has Next: %d\n", list_has_next(list));
  printf("Has Prev: %d\n", list_has_prev(list));
  if (list_get_value(list)==NULL)
    printf("Value NULL\n");
  list_add_value_to_tail(list, (void *)3, NULL);
  if (list_get_value(list)!=NULL)
    printf("Value: %d\n", list_get_value(list));

  list_free(list);
}