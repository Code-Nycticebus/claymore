#ifndef __CM_APP_H__
#define __CM_APP_H__

#define _CM_LOGGER_INTERNAL
#include "claymore/logger/logger.h"
#include "cm.h"

typedef struct ClaymoreConfig {
  struct {
    uint32_t width;
    uint32_t height;
    const char *title;
  } window;

  struct {
    cm_log_file *out;
    cm_log_file *err;
  } log;

} ClaymoreConfig;

typedef void WindowContext;

typedef struct {
  struct {
    uint32_t widht;
    uint32_t height;
    WindowContext *ctx;
  } window;

  struct {
    mat4 projection;
    mat4 view;
  } camera;

} ClaymoreApp;

#ifdef _CM_APP_INTERNAL
ClaymoreApp claymore_app_init(const ClaymoreConfig *config);
#endif /* !_CM_APP_INTERNAL */

extern ClaymoreConfig cm_app_config(void);
extern void cm_app_init(ClaymoreApp *app);
extern void cm_app_update(ClaymoreApp *app);
extern void cm_app_free(void);

#endif /* !__CM_APP_H__ */
