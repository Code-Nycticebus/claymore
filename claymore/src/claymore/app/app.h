#ifndef __CLAYMORE_APP_H__
#define __CLAYMORE_APP_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "RGFW.h"

#include "scene.h"

// Fixed update frequenzy (50hz)
#define CM_FIXED_DELTA 0.02f

// App config struct
typedef const struct {
  struct {
    const char *title;
    usize width;
    usize height;
  } window;         // Window config
  CmSceneInit root; // Function pointer to scene initializer function
} ClaymoreConfig;

// App context struct
typedef struct {
  CmGpu gpu;           // App lifetime gpu resources
  RGFW_window *window; // Window context
} CmApp;

/* ========= App public ========= */

// Gets app context
CmApp *cm_app(void);

// Get window context
RGFW_window *cm_app_window(void);

// Sets the app background color
void cm_app_background(const vec3 color);

// Returns app gpu context
CmGpu *cm_app_gpu(void);

// Returns the user data pointer
void *cm_app_data(void);
// Sets the user data pointer and allocates memory of given size
void *cm_app_set_data(usize size);

// Gets root scene
CmScene *cm_app_root(void);
// Sets root scene
CmScene *cm_app_set_root(CmSceneInit init);

// Gets deltatime
double cm_app_deltatime(void);
// Gets time the app is running
u64 cm_app_time(void);

// Quit the app
void cm_app_quit(void);

/* ========= App internal ========= */

bool cm_app_internal_init(ClaymoreConfig *config);
bool cm_app_internal_update(void);
void cm_app_internal_final(void);

void cm_app_internal_use(CmApp *app);

void cm_app_internal_event(CmEvent *event);

void cm_app_internal_schedule_delete(CmScene *scene);
void cm_app_internal_schedule_build(void);

#endif /* !__CLAYMORE_APP_H__ */
