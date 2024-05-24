#ifndef __CLAYMORE_APP_H__
#define __CLAYMORE_APP_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "RGFW.h"

#include "scene.h"

typedef const struct {
  struct {
    const char *title;
    usize width;
    usize height;
  } window;
  CmSceneInit main;
} ClaymoreConfig;

typedef struct {
  Arena arena;
  RGFW_window *window;
} CmApp;

CmApp *cm_app(void);
CmScene *cm_app_root(void);
RGFW_window *cm_app_window(void);
void *cm_app_alloc(usize size);

void cm_app_set_main(CmSceneInit init);
double cm_app_time(void);
void cm_app_quit(void);
void cm_app_background(const vec3 color);

// INTERNAL

bool cm_app_internal_init(ClaymoreConfig *config);
bool cm_app_internal_update(void);
void cm_app_internal_final(void);

void cm_app_internal_event(CmEvent *event);

#endif /* !__CLAYMORE_APP_H__ */
