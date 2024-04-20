#ifndef __CLAYMORE_APP_H__
#define __CLAYMORE_APP_H__

#include "claymore/defines.h" // IWYU pragma: export

#include "claymore/event/types.h"

#include "scene.h"

typedef const struct {
  struct {
    const char *title;
    usize width;
    usize height;
  } window;
  CmSceneInit main;
} ClaymoreConfig;

CmScene *cm_app_root(void);
void cm_app_set_main(CmSceneInit init);

// INTERNAL

bool cm_app_internal_init(ClaymoreConfig *config);
bool cm_app_internal_update(void);
void cm_app_internal_final(void);

void cm_app_internal_event(CmEvent *event);

#endif /* !__CLAYMORE_APP_H__ */
