
## Scene 

### Definition

You define a custom scene by implementing an interface with five functions: `init`, `fixed_update`, `frame_update`, `final`, and `event`. 
You can name these functions however you like, and they can also be `NULL` if not needed. 

```c
static void init(CmScene* scene) {
    // Runs once when the scene is initialized
}

static void fixed_update(CmScene* scene, double deltatime) {
    // Runs an update in a fixed interval
    // Do your physics updates here
}

static void frame_update(CmScene* scene, double deltatime) {
    // Runs every frame
    // Do your rendering here
}

static void final(CmScene* scene) {
    // Runs when the scene is removed
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

### Scene Structure

You can define a scene that uses custom data. Allocate data with `cm_scene_set_data()`. 
Then you can access `->data` from the scene.

```c
typedef struct {
    CmCamera2D camera;
    float radius;
} SceneData;

static void init(CmScene* scene) {
    // Allocate data
    SceneData* data = cm_scene_set_data(scene, sizeof(SceneData));
    // Initialize the data
    data->radius = 100.f;
    cm_camera2D_screen(&data->camera);
}

static void frame_update(CmScene* scene, double deltatime) {
    // Access data
    SceneData* data = scene->data;

    // Render data
    cm_2D_begin(&data->camera);
    cm_circle((vec2){0, 0}, data->radius, (vec4){1, 0, 0, 1});
    cm_2D_end();
}

// ...
```

### Creating a Scene Hierarchy

You can push a scene using `cm_scene_push()`, passing a function pointer that returns the `CmSceneInterface *`. 
Pushing will call the child's `init()`, and returns a pointer to the initialized scene. 
Parents are updated first, then in the order that scenes were pushed.

```c
static void init(CmScene* scene) {
    CmScene* child = cm_scene_push(scene, child_interface);
}
```

### Scene Initialization

If a scene is dependent on certain parameters, you can do something like this. 
You define a function that pushes the scene onto the parent and then initializes data with the parameters.

```c
CmScene* scene_init(CmScene* parent, float radius) {
    CmScene* scene = cm_scene_push(parent, scene_interface);
    SceneData* data = scene->data;

    // Initialize data based on parameters
    data->radius = radius;

    return scene;
}
```