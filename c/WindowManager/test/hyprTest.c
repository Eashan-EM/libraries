#include <stdlib.h>
#include "../windowmanager.h"

int main() {
  winman_setup();
  struct hyprland *hyprland = hyprland_instance();
  free(hyprland);
  winman_destroy();
}