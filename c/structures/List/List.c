#include <stdlib.h>
#include <string.h>
#include "List.h"

struct __ListStruct {
  struct __ListStruct *parent;
  void *value;
  void (*value_free_func)(void *);
  struct __ListStruct *child;
};

static struct __ListStruct *listS;

List *list_new() {
  List *newList = (List *) malloc(sizeof(List));
  memset(newList, 0, sizeof(List));
  return newList;
}

int list_free(List *list) {
  int freedAmount = 0;
  if (list!=NULL) {
    list_goto_tail(list);
    while (list_has_prev(list)) {
      listS = list->listCurrent;
      if (listS->value_free_func!=NULL)
        listS->value_free_func(listS->value);
      list_goto_prev(list);
      listS = list->listCurrent;
      free(listS->child);
      freedAmount++;
    }
    return freedAmount;
  }
  return 0;
}

int list_goto_head(List *list) {
  if (list!=NULL && list->listHead!=NULL) {
    list->listCurrent = list->listHead;
    return 1;
  }
  return 0;
}

int list_goto_tail(List *list) {
  if (list!=NULL && list->listTail!=NULL) {
    list->listCurrent = list->listTail;
    return 1;
  }
  return 0;
}

int list_goto_next(List *list) {
  if (list!=NULL && list->listCurrent!=NULL) {
    listS = list->listCurrent;
    if (listS->child!=NULL) {
      list->listCurrent = listS->child;
      return 1;
    }
  }
  return 0;
}

int list_goto_prev(List *list) {
  if (list!=NULL && list->listCurrent!=NULL) {
    listS = list->listCurrent;
    if (listS->parent!=NULL) {
      list->listCurrent = listS->parent;
      return 1;
    }
  }
  return 0;
}

int list_has_next(List *list) {
  if (list!=NULL && list->listCurrent!=NULL) {
    listS = list->listCurrent;
    if (listS->child!=NULL)
      return 1;
  }
  return 0;
}

int list_has_prev(List *list) {
  if (list!=NULL && list->listCurrent!=NULL) {
    listS = list->listCurrent;
    if (listS->parent!=NULL)
      return 1;
  }
  return 0;
}

void *list_get_value(List *list) {
  if (list!=NULL && list->listCurrent!=NULL) {
    listS = list->listCurrent;
    return listS->value;
  }
  return NULL;
}

int list_add_value_to_tail(List *list, void *value, void (*value_free_func)(void *)) {
  if (list!=NULL) {
    if (list->listHead==NULL) {
      list->listHead = (struct __ListStruct *) malloc(sizeof(struct __ListStruct));
      memset(list->listHead, 0, sizeof(struct __ListStruct));
      list->listTail = list->listHead;
      list->listCurrent = list->listHead;
      listS = list->listHead;
    } else {
      listS = list->listTail;
      listS->child = (struct __ListStruct *) malloc(sizeof(struct __ListStruct));
      memset(listS->child, 0, sizeof(struct __ListStruct));
      listS->child->parent = listS;
      listS = listS->child;
    }
    listS->value = value;
    listS->value_free_func = value_free_func;
    list->listTail = listS;
    return 1;
  }
  return 0;
}