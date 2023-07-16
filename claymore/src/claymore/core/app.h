#ifndef __CM_APP_H__
#define __CM_APP_H__

#include "claymore/renderer/renderer2D.h"
#include "cm.h"

typedef struct ClaymoreConfig {
  struct {
    uint32_t width;
    uint32_t height;
    const char *title;
  } window;

} ClaymoreConfig;

typedef void WindowContext;

typedef struct {
  struct {
    uint32_t widht;
    uint32_t height;
    WindowContext *ctx;
  } window;

  Renderer2D renderer;

} ClaymoreApp;

#ifdef _CM_APP_INTERNAL
ClaymoreApp claymore_app_init(const ClaymoreConfig *config);
#endif /* !_CM_APP_INTERNAL */

extern ClaymoreConfig cm_app_config(void);
extern void cm_app_init(ClaymoreApp *app);
extern void cm_app_update(ClaymoreApp *app);
extern void cm_app_free(void);

#endif /* !__CM_APP_H__ */
