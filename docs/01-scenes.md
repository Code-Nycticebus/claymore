
## Scene 

### Definition

You define a custom scene by implementing an interface with 
five functions: `init`, `fixed_update`, `frame_update`, `final`, and `event`. 
You can name these functions however you like, and they can also be `NULL` if not needed. 
The `CmSceneInterface *scene_interface(void);` function returns a pointer to a `CmSceneInterface` struct.

```c
static void init(CmScene* scene) {
    // Runs once when the scene is initialized
}

static void fixed_update(CmScene* scene, double deltatime) {
    // Runs an update in a fixed interval
}

static void frame_update(CmScene* scene, double deltatime) {
    // Runs every frame
}

static void final(CmScene* scene) {
    // Runs when scene is removed
}

static void event(CmScene* scene, CmEvent* event) {
    // Runs on event
}

CmSceneInterface *scene_interface(void) {
  static CmSceneInterface interface = {
      .init = init,
      .fixed_update = fixed_update,
      .frame_update = frame_update,
      .final = final,
      .event = event,
  };
  return &interface;
}
```


### Common Scene Structure

You can define a scene that uses custom data in a `SceneData` struct. 
Allocate data with `cm_scene_set_data()`.

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

static void frame_update(CmScene* scene, double deltatime) {
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

If a scene is dependent on certain parameters you can do something like this.
You define a function that pushes the scene onto the parrent and then initializes data with the parameters.

```c
CmScene* scene_init(CmScene* parent, float radius) {
    CmScene* scene = cm_scene_push(parent, scene_interface);
    SceneData* data = scene->data;

    data->radius = radius;

    return scene;
}
```
