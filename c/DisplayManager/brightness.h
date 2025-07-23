typedef struct Brightness Brightness;
struct BrightnessCallbacks {
  void (*brightness_update_cb)(Brightness *, unsigned, void *);
  void *brightness_update_data;
  void (*brightness_update_change_cb)(Brightness *, unsigned, void *);
  void *brightness_update_change_data;
};
typedef struct BrightnessCallbacks BCbs;

struct Brightness {
  char *backlight_path;
  char *max_brightness_path;
  char *brightness_path;

  unsigned int min_brightness;
  unsigned int max_brightness;
  unsigned int current_brightness;
  BCbs bcbs;
};

enum BType {
  Literal,
  Percent,
  Normalized
};

Brightness *brightness_new();
int brightness_free(Brightness *);
int brightness_set(struct Brightness *, enum BType, float);
float brightness_get(Brightness *, enum BType);
unsigned brightness_get_max(Brightness *);
float brightness_convert_to(Brightness *, enum BType, enum BType, float);
int brightness_set_cb_update(Brightness *, void (*)(Brightness *, unsigned, void *), void *);
int brightness_set_cb_update_change(Brightness *, void (*)(Brightness *, unsigned, void *), void *);
int brightness_update(Brightness *);