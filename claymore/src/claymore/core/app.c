#include "app.h"

#include "claymore/core/window.h"
#include "claymore/debug/debug.h"
#include "claymore/renderer/context.h"

ClaymoreApp claymore_app_init(const ClaymoreConfig *config, int *res) {
  ClaymoreApp app;
  app.window.widht = config->window.width;
  app.window.height = config->window.height;
  app.window.ctx = cm_window_init(config->window.width, config->window.height,
                                  config->window.title);

  if (!cm_context_init(app.window.ctx)) {
    return (ClaymoreApp){0};
  }

  if (app.window.ctx == NULL) {
    *res = -1;
    return (ClaymoreApp){0};
  }

  printf("%s\n", cm_debug_enabled ? "Debug" : "Release");

  return app;
}
