#ifndef __CM_APP_H__
#define __CM_APP_H__

#include "claymore/core/window.h"
#include "claymore/events/event.h"

typedef struct ClaymoreConfig {
  struct {
    uint32_t width;
    uint32_t height;
    const char *title;
  } window;

} ClaymoreConfig;

typedef struct {
  CMwindow *window;

  struct {
    mat4 projection;
    mat4 view;
  } camera;

  bool run;
} CmApp;

#ifdef _CM_APP_INTERNAL
bool cm_app_init(CmApp *app, const ClaymoreConfig *config);
void cm_app_shutdown(CmApp *app);
#endif /* !_CM_APP_INTERNAL */

/* User functions! */
extern ClaymoreConfig claymore_config(void);
extern void claymore_init(CmApp *app);
extern void claymore_update(CmApp *app);
extern void claymore_free(CmApp *app);

#endif /* !__CM_APP_H__ */
