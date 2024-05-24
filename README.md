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

## Usage

# Scene Structure
```c
typedef struct {
    // Data
} SceneData;

static void init(CmScene* scene) {
    SceneData* data = cm_scene_alloc(scene, sizeof(SceneData));
    // ...
}

static void update(CmScene* scene, double deltatime) {
    // draw here
}

static CmSceneInterface *scene(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
      .event = NULL,
      .final = NULL,
  };
  return &interface;
}
```

# Entry Point

```c
#include <claymore/entrypoint.h>

static ClaymoreConfig *claymore_init(void) {
  static ClaymoreConfig config = {
      .window = {.width = 720, .height = 420, .title = "Claymore"},
      .main = scene,
  };
  return &config;
}
```

# Scene Initialization
```c
CmScene* shader_init(CmScene* parent, ...) {
    CmScene* scene = cm_scene_push(parent, scene_interface);
    SceneData* data = scene->data;

    // Initializing SceneData based on parameters

    return scene;
}
```
