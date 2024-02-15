#ifndef __CLAYMORE_APP_H__
#define __CLAYMORE_APP_H__

#include "claymore/defines.h" // IWYU pragma: export
#include "claymore/event/types.h"

#include "scene.h"

typedef struct {
  struct {
    const char *title;
    usize width;
    usize height;
  } window;
  CmSceneInit scene;
} ClaymoreConfig;

CmScene *app_root(void);

// INTERNAL

bool app_internal_init(const ClaymoreConfig *config);
bool app_internal_update(void);
void app_internal_terminate(void);

void app_internal_on_event(CmEvent *event);

#endif /* !__CLAYMORE_APP_H__ */
