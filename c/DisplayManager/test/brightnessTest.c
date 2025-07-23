#include "../brightness.c"

int main() {
  Brightness *bright = brightness_new();
  printf("%d\n", brightness_set(bright, Percent, 40));
  printf("%f\n", brightness_get(bright, Percent));
  printf("%d\n", brightness_set(bright, Literal, 11000));
  printf("%d\n", brightness_set(bright, Normalized, 0.6));
  printf("%d\n", brightness_set(bright, Percent, 400));
  printf("%d\n", brightness_set(bright, Literal, 30000));
  printf("%d\n", brightness_set(bright, Normalized, 2.5));
  printf("%d\n", brightness_set(bright, Normalized, 0));
  brightness_free(bright);
}