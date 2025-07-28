#include "hyprland.h"
#include <json.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static struct hyprland *hyprland;

int dispatch_workspace(int index) {
  char command[30];
  sprintf(command, "hyprctl dispatch workspace %d", index);
  int output = system(command);
  return output;
}

int hyprland_functions_setup() {
  hyprland = malloc(sizeof(struct hyprland));
  hyprland->dispatch.workspace = dispatch_workspace;
  return 0;
}

int hyprland_setup() {
  hyprland_functions_setup();
  json_object *jobj = NULL;
  const char *mystring = NULL;
  int stringlen = 0;
  struct json_tokener *tok = json_tokener_new_ex(5);
  enum json_tokener_error jerr;

  FILE *pipe;
  char buffer[1024] = {'\0'};
  char command[] = "hyprctl workspaces -j";
  pipe = popen(command, "r");
  if (pipe==NULL) {
    return -1;
  }
  do {
    stringlen = strlen(buffer);
    jobj = json_tokener_parse_ex(tok, buffer, stringlen);
  } while (fgets(buffer, sizeof(buffer), pipe)!=NULL);
  if (jobj==NULL)
    return -1;
  return 0;
}

struct hyprland *hyprland_instance() {
  return hyprland;
}