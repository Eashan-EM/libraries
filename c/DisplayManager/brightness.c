#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "brightness.h"

Brightness *brightness_new() {
  Brightness *new = malloc(sizeof(Brightness));
  new->backlight_path = "/sys/class/backlight/intel_backlight";
  new->max_brightness_path = "/sys/class/backlight/intel_backlight/max_brightness";
  new->brightness_path = "/sys/class/backlight/intel_backlight/brightness";

  FILE *fd = fopen(new->max_brightness_path, "r");
  if (fd==NULL)
    return NULL;
  fscanf(fd, "%u", &new->max_brightness);
  fclose(fd);
  fd = fopen(new->brightness_path, "r");
  if (fd==NULL)
    return NULL;
  fscanf(fd, "%u", &new->current_brightness);
  new->min_brightness = 0.1*new->max_brightness;
  
  return new;
}

int brightness_free(Brightness *bright) {
  free(bright);
}

int brightness_set(struct Brightness *bright, enum BType type, float value) {
  value = brightness_convert_to(bright, Literal, type, value);
  value = (unsigned) (value<bright->min_brightness)?bright->min_brightness:value;
  value = (unsigned) (value>bright->max_brightness)?bright->max_brightness:value;

  errno = 0;
  FILE *fd = fopen(bright->brightness_path, "w");
  if (fd==NULL)
    return errno;
  fprintf(fd, "%g", value);
  bright->current_brightness = value;
  fclose(fd);
  return 0;
}

float brightness_get(Brightness *bright, enum BType type) {
  return brightness_convert_to(bright, type, Literal, bright->current_brightness);
}

int brightness_inc(Brightness *bright, enum BType type, float value) {
  value = brightness_convert_to(bright, Literal, type, value);
  if (value==-1)
    return 1;
  value += bright->current_brightness;
  brightness_set(bright, Literal, value);
  return 0;
}

int brightness_dec(Brightness *bright, enum BType type, float value) {
  value = brightness_convert_to(bright, Literal, type, value);
  if (value==-1)
    return 1;
  value = bright->current_brightness-value;
  brightness_set(bright, Literal, value);
  return 0;
}

unsigned brightness_get_max(Brightness *bright) {
  return bright->max_brightness;
}

float brightness_convert_to(Brightness *bright, enum BType to, enum BType from, float value) {
  switch (from) {
    case Literal:
      break;
    case Percent:
      value = (unsigned) (value*bright->max_brightness/100);
      break;
    case Normalized:
      value = (unsigned) (value*bright->max_brightness);
      break;
    default:
      return -1; 
  }
  switch (to) {
    case Literal:
      return value;
    case Percent:
      return (100*value/(float)bright->max_brightness);
    case Normalized:
      return (value/(float)bright->max_brightness);
    default:
      return -1;
  }
}

int brightness_set_cb_update(Brightness *bright, void (*func)(Brightness *, unsigned, void *), void *data) {
  bright->bcbs.brightness_update_cb = func;
  bright->bcbs.brightness_update_data = data;
  return 0;
}

int brightness_set_cb_update_change(Brightness *bright, void (*func)(Brightness *, unsigned, void *), void *data) {
  bright->bcbs.brightness_update_change_cb = func;
  bright->bcbs.brightness_update_change_data = data;
  return 0;
}

int brightness_update(Brightness *bright) {
  unsigned value;
  errno = 0;
  FILE *fd = fopen(bright->brightness_path, "r");
  if (fd==NULL)
    return errno;
  fscanf(fd, "%u", &value);

  fclose(fd);

  if (value!=bright->current_brightness) {
    if (bright->bcbs.brightness_update_cb!=NULL)
      bright->bcbs.brightness_update_cb(
        bright,
        value, 
        bright->bcbs.brightness_update_data
      );
    if (bright->bcbs.brightness_update_change_cb!=NULL)
      bright->bcbs.brightness_update_change_cb(
        bright,
        (value>bright->current_brightness)?(value-bright->current_brightness):(bright->current_brightness-value),
        bright->bcbs.brightness_update_change_data
      );
    bright->current_brightness = value;
  } 
  return 0;
}