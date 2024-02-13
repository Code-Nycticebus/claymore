#ifndef __CLAYMORE_APP_H__
#define __CLAYMORE_APP_H__

#include "claymore/app/scene.h"
#include "claymore/defines.h" // IWYU pragma: export

typedef struct {
  struct {
    const char *title;
    usize width;
    usize height;
  } window;
  CmSceneInit scene;
} ClaymoreConfig;

bool app_init(const ClaymoreConfig *config);
bool app_update(void);
void app_terminate(void);

#endif /* !__CLAYMORE_APP_H__ */
