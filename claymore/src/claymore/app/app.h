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

CmScene *app_root(void);

// INTERNAL

bool app_internal_init(ClaymoreConfig *config);
bool app_internal_update(void);
void app_internal_final(void);

void app_internal_event(CmEvent *event);

#endif /* !__CLAYMORE_APP_H__ */
