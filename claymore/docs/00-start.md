# Claymore

## Entry Point

Include `claymore/entrypoint.h` in only one  `.c` file. In this file, implement `ClaymoreConfig* claymore_init(void);`. 
This function should return a pointer to a `ClaymoreConfig` structure, 
where you define the window information and specify the main [scene](https://github.com/Code-Nycticebus/claymore/blob/main/claymore/docs/01-scenes.md).

```c
#include <claymore/entrypoint.h>

static ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Claymore"},
      .main = scene_interface,
  };
  return &config;
}
```

