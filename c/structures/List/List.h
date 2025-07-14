struct ListHandler {
  void *listHead;
  void *listTail;
  void *listCurrent; 
};

typedef struct ListHandler List;
typedef void (*value_free_func)(void *);

List *list_new();
int list_free(List *);
int list_goto_head(List *);
int list_goto_tail(List *);
int list_goto_next(List *);
int list_goto_prev(List *);
int list_has_next(List *);
int list_has_prev(List *);
void *list_get_value(List *);
int list_add_value_to_tail(List *, void *, value_free_func);
int list_add_value_to(List *, int);
int list_free_value(List *, value_free_func);
int list_free_value_to(List *, value_free_func, int);
int list_length(List *);