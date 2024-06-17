#ifndef __CLAYMORE_APP_H__
#define __CLAYMORE_APP_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "RGFW.h"

#include "scene.h"

#define CM_FIXED_DELTA 0.02f // 50hz

typedef const struct {
  struct {
    const char *title;
    usize width;
    usize height;
  } window;
  CmSceneInit root;
} ClaymoreConfig;

typedef struct {
  void *data;
  RGFW_window *window;
} CmApp;

CmApp *cm_app(void);
CmScene *cm_app_root(void);
RGFW_window *cm_app_window(void);
void *cm_app_data(void);
void *cm_app_set_data(usize size);
double cm_app_deltatime(void);

CmScene *cm_app_set_main(CmSceneInit init);
u64 cm_app_time(void);
void cm_app_quit(void);
void cm_app_background(const vec3 color);

// INTERNAL

bool cm_app_internal_init(ClaymoreConfig *config);
bool cm_app_internal_update(void);
void cm_app_internal_final(void);

void cm_app_internal_event(CmEvent *event);

void cm_app_internal_schedule_delete(CmScene *scene);
void cm_app_internal_schedule_build(void);

#endif /* !__CLAYMORE_APP_H__ */
