struct hypr_dispatch {
  int (*workspace)(int);
};

struct hypr_workspaces {
  int id;
  const char *name;
  const char *monitor;
  int monitor_id;
  int windows;
  int has_full_screen;
  const char *last_window;
  const char *last_window_title;
};

struct hyprland {
  struct hypr_dispatch dispatch;
  struct hypr_workspaces active_workspace;
  ;
};

int hyprland_setup();
struct hyprland *hyprland_instance();