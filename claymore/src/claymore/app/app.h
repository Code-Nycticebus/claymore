#ifndef __CLAYMORE_APP_H__
#define __CLAYMORE_APP_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "RGFW.h"

#include "claymore/renderer/2D/renderer2D.h"
#include "scene.h"

// fixed update frequenzy (50hz)
#define CM_FIXED_DELTA 0.02f

/* App Config (ClaymoreConfig)
 *  - Configuration of the main app, including window settings and the
 *    root scene initializer.
 */
typedef const struct {
  struct {
    const char *title; // window title
    usize width;       // width
    usize height;      // height
    u16 args;          // optional arguments (RGFW.h:247)
  } window;            // window config
  CmSceneInit root;    // function pointer to scene initializer function
} ClaymoreConfig;

/* App Context (CmApp)
 *  - App-wide resources, including memory (arnea), GPU resources and window
 * context.
 */
typedef struct {
  CmGpu gpu;           // app lifetime gpu resources
  Arena arena;         // app lifetime arena
  RGFW_window *window; // window context
} CmApp;

/* ========= app public ========= */

// gets app context
CmApp *cm_app(void);

// get window context
RGFW_window *cm_app_window(void);
// get window aspect ratio
float cm_app_aspect(void);

// sets the app background color
void cm_app_background(const vec3 color);

// returns app gpu context
CmGpu *cm_app_gpu(void);

// returns the user data pointer
void *cm_app_data(void);
// sets the user data pointer and allocates memory of given size
void *cm_app_set_data(usize size);

// gets root scene
CmScene *cm_app_root(void);
// sets root scene
CmScene *cm_app_set_root(CmSceneInit init);

// gets deltatime
double cm_app_deltatime(void);
// gets time the app is running
u64 cm_app_time(void);

// quit the app with an exit code
void cm_app_quit(i32 code);

/* ========= app internal ========= */

/* Internal App Context (CmAppInternal)
 *  - Internal state and management for the app, including public context,
 *    memory management, and scene handling.
 * */
typedef struct {
  CmApp public; // public app context
  Arena arena;  // app lifetime arena
  bool running; // running state

  u64 first_frame; // timestamp of first frame

  u64 last_frame;   // timestamp of last frame
  double deltatime; // deltatime

  CmSceneInternal *root;         // current root scene
  CmSceneInternal *new_root;     // scheduled new scene
  DA(CmSceneInternal *) deleted; // scheduled to delete scenes

  CmRenderer2D *renderer; // 2D renderer context
} CmAppInternal;

bool cm_app_internal_init(ClaymoreConfig *config);
bool cm_app_internal_update(void);
void cm_app_internal_final(void);

void cm_app_internal_use(CmApp *app);

void cm_app_internal_event(CmEvent *event);

void cm_app_internal_schedule_delete(CmScene *scene);

#endif /* !__CLAYMORE_APP_H__ */
