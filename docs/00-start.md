# Claymore

## Build Project

### C Compiler

To compile and build the project using a C compiler, you need to create a build script, such as `build.c`. 
I'm assuming here that `claymore` is inside the `libs` directory.

Follow these steps to create and use the build script.
```c
#ifndef CC
#define CC "gcc" // Define the C compiler
#endif

// Provide claymore directory.
// This has to be relative to the working directory
#define CM_DIR "libs/claymore"
#include "libs/claymore/build/claymore.h"

int main(void) {
  claymore_project_build_and_run(PATH("project"), PATH("."), /* rebuild = */ true);
}
```

Compile and run build system
```terminal
gcc build.c && ./a.out
```

### Pybuildc

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

### Prebuild binaries

> :warning: Not implemented yet!


## Entry Point

Include `claymore/entrypoint.h` in only one  `.c` file. In this file, implement `ClaymoreConfig* claymore_init(void);`. 

```c
#include <claymore/entrypoint.h>

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Claymore"},
      .root = scene_interface,
  };
  return &config;
}
```
