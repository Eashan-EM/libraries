#include "../brightness.h"
#include <stdio.h>

void update_cb(Brightness *bright, unsigned value, void *data) {
  printf("Brightness Changed: %g%%\n", brightness_get(bright, Percent));
}

void update_change_cb(Brightness *bright, unsigned value, void *data) {
  printf("Brightness Changed By: %g%%\n", brightness_convert_to(bright, Percent, Literal, value));
}

int main() {
  Brightness *bright = brightness_new();
  brightness_set_cb_update(bright, update_cb, NULL);
  brightness_set_cb_update_change(bright, update_change_cb, NULL);
  while (1)
    brightness_update(bright);
  brightness_free(bright);
}