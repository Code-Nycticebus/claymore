# claymore
A C Game Engine

## Why claymore
Just don't. There are way better opengl frameworks out there. 
This is just a fun little project.

## Setup
First clone the repo:

```terminal
git clone https://github.com/Code-Nycticebus/claymore
cd claymore
```

Run these scripts to build the dependencies.

#### Linux
```
cd claymore/deps
sh build_debian.sh
```

#### Windows
```
cd claymore/deps
.\build_win32.bat
```


## Start a project
Claymore uses my own build system [pybuildc](https://github.com/Code-Nycticebus/pybuildc)

```
pybuildc new test-project
```

and add claymore as a dependency:

```toml
[deps.claymore]
dir="<CLAYMORE DIRECTORY>" 
type="pybuildc"
```


To get a quick program started:

```c
#include "claymore/entrypoint.h"

static void init(CmScene *scene) {
  (void)scene;
}

static CmSceneInterface *scene_init(void) {
  static CmSceneInterface scene = {
      .init = init,
  };
  return &scene;
}

const ClaymoreConfig *claymore_init(void) {
  static const ClaymoreConfig config = {
      .window =
          {
              .width = 720,
              .height = 420,
              .title = "test project",
          },
      .scene = scene_init,
  };
  return &config;
}
```
