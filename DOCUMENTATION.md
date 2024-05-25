# Claymore

## Entry Point

Include `claymore/entrypoint.h` in only one  `.c` file. In this file, implement `ClaymoreConfig* claymore_init(void);`. 
This function should return a pointer to a `ClaymoreConfig` structure, where you define the window information and specify the main [scene](#Scene).

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

## Scene 

### Definition

You define a custom scene by implementing an interface with 
four main functions: `init`, `update`, `final`, and `event`. 
You can name these functions however you like, and they can also be `NULL` if not needed. 
The `CmSceneInterface *scene_interface(void);` function returns a pointer to a `CmSceneInterface` struct.

```c
static void init(CmScene* scene) {
    // Initialization code...
}

static void update(CmScene* scene, double deltatime) {
    // Update logic
}

static void final(CmScene* scene) {
    // Cleanup code
}

static void event(CmScene* scene, CmEvent* event) {
    // Event handling code...
}

static CmSceneInterface *scene_interface(void) {
  static CmSceneInterface interface = {
      .init = init,
      .update = update,
      .final = final,
      .event = event,
  };
  return &interface;
}
```


### Common Scene Structure

You can define a scene that uses custom data in a `SceneData` struct. 

```c
typedef struct {
    CmCamera2D camera;
    float radius;
} SceneData;

static void init(CmScene* scene) {
    SceneData* data = cm_scene_set_data(scene, sizeof(SceneData));
    data->radius = 100.f;
    cm_camera2D_screen(&data->camera);
}

static void update(CmScene* scene, double deltatime) {
    SceneData* data = scene->data;
    cm_2D_begin(&data->camera);
    cm_circle((vec2){0, 0}, data->radius, (vec4){1, 0, 0, 1});
    cm_2D_end();
}
```


### Creating a Scene Hierarchy

You can push a scene which will call the child scenes `init()`, and returns a pointer to the initialized scene.
Parents are updated first, then the order that scenes where pushed.

```c
static void init(CmScene* scene) {
    CmScene* child = cm_scene_push(scene, child_interface);
}
```


### Scene Initialization

Since its not possible to pass parameters to the init function i came up with this solution.
you define a function that pushes the scene onto the parrent.

```c
CmScene* scene_init(CmScene* parent, float radius) {
    CmScene* scene = cm_scene_push(parent, scene_interface);
    SceneData* data = scene->data;

    data->radius = radius;

    return scene;
}
```
