struct hypr_dispatch {
  int (*workspace)(int);
};

struct hypr_workspaces {
  int active_workspace;
};

struct hyprland {
  struct hypr_dispatch dispatch;
  struct hypr_workspaces workspaces;
};

int hyprland_setup();
struct hyprland *hyprland_instance();