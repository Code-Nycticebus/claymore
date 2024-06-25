#ifndef __CLAYMORE_SCENE_H__
#define __CLAYMORE_SCENE_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/app/event.h"
#include "claymore/renderer/gpu.h"

/* ========= scene public ========= */

/* Public Scene Data
 *  - Public data mostly for resource management within a scene,
 *    including memory (arena) and GPU resources (gpu).
 * */
typedef struct {
  Arena arena;
  CmGpu gpu;
} CmScene;

// helper to initialize a scene interface with its name and size
#define CM_SCENE(T) .name = STR_STATIC(#T), .size = sizeof(T)

/* Scene Interface:
 *  - 'name' and 'size' store metadata about the scene type.
 *  - Function pointers for scene lifecycle events (init, updates, event
 *    handling, finalization).
 * */
typedef const struct {
  // Metadata
  Str name;
  usize size;

  // function pointer for scene initialization
  void (*init)(CmScene *scene);
  // function pointer for fixed updates (e.g., physics)
  void (*fixed_update)(CmScene *scene);

  // function pointer for frame updates (e.g., rendering)
  void (*frame_update)(CmScene *scene);
  // function pointer for pre-update processing
  void (*pre_update)(CmScene *scene);
  // function pointer for post-update processing
  void (*post_update)(CmScene *scene);

  // function pointer for finalization (cleanup)
  void (*final)(CmScene *scene);
  // function pointer for handling events
  void (*event)(CmScene *scene, CmEvent *event);
} CmSceneInterface;

// typedef for scene interface initialization function
typedef const CmSceneInterface *(*CmSceneInit)(void);

// typedef for scene dynamic array
typedef DA(CmScene *) CmSceneDa;

// adds child to parent scene
CmScene *cm_scene_push(CmScene *scene, CmSceneInit init);
// deletes itself from the scene tree
void cm_scene_delete(CmScene *scene);

// getters
void *cm_scene_data(CmScene *scene);
Str cm_scene_name(CmScene *scene);
CmScene *cm_scene_parent(CmScene *scene);
CmScene *cm_scene_child(CmScene *scene, usize idx);
const CmSceneDa *cm_scene_children(CmScene *scene);

// find first occurence of scene
CmScene *cm_scene_find(CmScene *root, Str name);
// find all occurences of scene
void cm_scene_find_all(CmSceneDa *out, CmScene *root, Str name);

/* ========= scene internal ========= */

/* Internal Scene Context (CmSceneInternal)
 *  - Internal state of a scene, including user-specific data and private data
 *    for managing lifecycle, resources, and hierarchy.
 * */
typedef struct CmSceneInternal {
  CmScene public;                        // public scene context
  Arena arena;                           // scene lifetime arena
  const CmSceneInterface *interface;     // vtable
  struct CmSceneInternal *parent;        // parent
  DA(struct CmSceneInternal *) children; // children
  CGLM_ALIGN_MAT u8 data[];              // user data
} CmSceneInternal;

CmSceneInternal *cm_scene_internal_init(Arena *arena, CmSceneInit init);

void cm_scene_internal_final(Arena *arena, CmSceneInternal *scene);
void cm_scene_internal_event(CmSceneInternal *scene, CmEvent *event);

#endif /* !__CLAYMORE_SCENE_H__ */
