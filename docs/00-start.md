# Claymore

## Pybuildc

Claymore uses my own build system [pybuildc](https://github.com/Code-Nycticebus/pybuildc)

```
pybuildc new test-project
```

and add claymore as a dependency to the `pybuildc.toml`:

```toml
[libs.claymore]
dir="<CLAYMORE DIRECTORY>" 
type="pybuildc"
```

## Prebuild binaries

> :warning: Not implemented yet!

## Entry Point

Include `claymore/entrypoint.h` in only one  `.c` file. In this file, implement `ClaymoreConfig* claymore_init(void);`. 

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

