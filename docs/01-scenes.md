
## Scene 

### Definition

You define a custom scene by implementing an interface with five functions: `init`, `fixed_update`, `frame_update`, `final`, and `event`. 
You can name these functions however you like, and they can also be `NULL` if not needed. 

```c
typedef struct {
    // ...
} SceneData;

static void init(CmScene* scene) {
    // Runs once when the scene is initialized
}

static void fixed_update(CmScene* scene) {
    // Runs an update in a fixed interval
    // Do your physics updates here
}

static void frame_update(CmScene* scene) {
    // Runs every frame
    // Do your rendering here
}

static void final(CmScene* scene) {
    // Runs when the scene is removed
}

static void event(CmScene* scene, CmEvent* event) {
    // Runs on event
}

static CmSceneInterface *scene_interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(SceneData),
      .fixed_update = fixed_update,
      .frame_update = frame_update,
      .final = final,
      .event = event,
  };
  return &interface;
}
```

### Scene Structure

You can define a scene that uses custom data. Allocate data by providing the interface with a size. 
Then you can access the data using `cm_scene_data(scene)`.

```c
typedef struct {
    CmCamera2D camera;
    float radius;
} SceneData;

static void init(CmScene* scene) {
    // Access data
    SceneData* data = cm_scene_data(scene);
    // Initialize the data
    data->radius = 100.f;
    cm_camera2D_screen(&data->camera);
}

static void frame_update(CmScene* scene) {
    // Access data
    SceneData* data = cm_scene_data(scene);

    // Render data
    cm_2D_begin(&data->camera);
    cm_2D_circle((vec2){0, 0}, data->radius, (vec4){1, 0, 0, 1});
    cm_2D_end();
}

CmSceneInterface *scene_interface(void) {
  static CmSceneInterface interface = {
      CM_SCENE(SceneData),
      .init = init,
      .frame_update = frame_update,
  };
  return &interface;
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
    SceneData* data = cm_scene_data(scene);

    // Initialize data based on parameters
    data->radius = radius;

    return scene;
}
```

### Memory Management

To allocate memory within a scene, use the scene's [arena](https://github.com/Code-Nycticebus/cebus#arenah). 
This ensures the memory will be automatically freed when the scene is de-initialized.

```c
typedef struct {
    int* values;
} SceneData;

static void init(CmScene* scene) {
    // Access scene data
    SceneData* data = cm_scene_data(scene, sizeof(SceneData));
    // Allocate values
    data->values = arena_calloc(&scene->arena, sizeof(int) * 3);
    // ...
}
```

### Ressource Management

You pass an `CmGpu` instance to every function that takes up ressources on the gpu. The gpu ressource will be freed after a scene is deinitialized.

```c
static void init(CmScene* scene) {
    // Access scene data
    SceneData* data = cm_scene_data(scene, sizeof(SceneData));
    // Create texture
    data->texture = cm_texture_from_file(&scene->gpu, STR("assets/texture.png"), ErrPanic);
    // ...
}
```

