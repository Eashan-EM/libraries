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

struct json_object *json_parse(char *command) {
  json_object *jobj = NULL;
  const char *mystring = NULL;
  int stringlen = 0;
  struct json_tokener *tok = json_tokener_new_ex(5);
  enum json_tokener_error jerr;

  FILE *pipe;
  char buffer[1024] = {'\0'};
  pipe = popen(command, "r");
  if (pipe==NULL) {
    return NULL;
  }
  do {
    stringlen = strlen(buffer);
    jobj = json_tokener_parse_ex(tok, buffer, stringlen);
  } while (fgets(buffer, sizeof(buffer), pipe)!=NULL);
  if (jobj==NULL)
    return NULL;
  return jobj;
}

int hyprland_workspaces() {
 struct json_object *workspace_array = json_parse("hyprctl workspaces -j");
 int arrayLen = json_object_array_length(workspace_array);
 struct json_object *workspace, *temp_obj;
 int val;
 if (json_object_get_type(workspace_array)==json_type_array) {
   for (int i=0; i<arrayLen; i++) {
    workspace = json_object_array_get_idx(workspace_array, i);
    if (json_object_get_type(workspace)==json_type_object) {
      val = json_object_object_get_ex(workspace, "id", &temp_obj);
    }
   }
 } else {
  return -1;
 }
 return 0;
}

int hyprland_active_workspace() {
  struct json_object *active_work = json_parse("hyprctl activeworkspace -j");
  struct json_object *temp_obj;
  int val;
  if (json_object_get_type(active_work)==json_type_object) {
    val = json_object_object_get_ex(active_work, "id", &temp_obj);
    if (json_object_is_type(temp_obj, json_type_int)) {
      hyprland->active_workspace.id = json_object_get_int(temp_obj);
    }
    val = json_object_object_get_ex(active_work, "name", &temp_obj);
    if (json_object_is_type(temp_obj, json_type_string)) {
      hyprland->active_workspace.name = json_object_get_string(temp_obj);
    }
    val = json_object_object_get_ex(active_work, "monitor", &temp_obj);
    if (json_object_is_type(temp_obj, json_type_string)) {
      hyprland->active_workspace.monitor = json_object_get_string(temp_obj);
    }
    val = json_object_object_get_ex(active_work, "monitorID", &temp_obj);
    if (json_object_is_type(temp_obj, json_type_int)) {
      hyprland->active_workspace.monitor_id = json_object_get_int(temp_obj);
    }
    val = json_object_object_get_ex(active_work, "windows", &temp_obj);
    if (json_object_is_type(temp_obj, json_type_int)) {
      hyprland->active_workspace.windows = json_object_get_int(temp_obj);
    }
    val = json_object_object_get_ex(active_work, "hasfullscreen", &temp_obj);
    if (json_object_is_type(temp_obj, json_type_boolean)) {
      hyprland->active_workspace.has_full_screen = json_object_get_boolean(temp_obj);
    }
    val = json_object_object_get_ex(active_work, "lastwindow", &temp_obj);
    if (json_object_is_type(temp_obj, json_type_string)) {
      hyprland->active_workspace.last_window = json_object_get_string(temp_obj);
    }
    val = json_object_object_get_ex(active_work, "lastwindowtitle", &temp_obj); 
    if (json_object_is_type(temp_obj, json_type_string)) {
      hyprland->active_workspace.last_window_title = json_object_get_string(temp_obj);
    }
    //printf("%d\n%s\n%s\n%d\n%d\n%d\n%s\n%s\n", hyprland->active_workspace.id, hyprland->active_workspace.name, hyprland->active_workspace.monitor, hyprland->active_workspace.monitor_id, hyprland->active_workspace.windows, hyprland->active_workspace.has_full_screen, hyprland->active_workspace.last_window, hyprland->active_workspace.last_window_title);
  } 
}

int hyprland_setup() {
  hyprland_functions_setup();
  hyprland_workspaces();
  hyprland_active_workspace();
 return 0;
}

struct hyprland *hyprland_instance() {
  return hyprland;
}