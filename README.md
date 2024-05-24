# claymore
A C Game Engine

> ⚠️ The Project as well as the Documentation is not yet finished.

## Setup
First clone the repo:

```terminal
git clone https://github.com/Code-Nycticebus/claymore --recurse-submodules 
```


## Start a project
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


To get a quick program started:

```c
#include "claymore/entrypoint.h"

static void init(CmScene *scene) {
  (void)scene;
}

static CmSceneInterface *scene(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = NULL,
      .event = NULL,
      .final = NULL,
  };
  return &interface;
}

ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window =
          {
              .width = 720,
              .height = 420,
              .title = "test project",
          },
      .main = scene,
  };
  return &config;
}
```
