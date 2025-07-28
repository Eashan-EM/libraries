#include <stdlib.h>
#include "../windowmanager.h"

int main() {
  winman_setup();
  struct hyprland *hyprland = hyprland_instance();
  hyprland->dispatch.workspace(2);
  free(hyprland);
  winman_destroy();
}