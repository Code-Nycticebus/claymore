#include "app.h"

#include "claymore/core/window.h"

ClaymoreApp claymore_app_init(const ClaymoreConfig *config, int *res) {
  ClaymoreApp app;
  app.window.widht = config->window.width;
  app.window.height = config->window.height;
  app.window.ctx = cm_window_init(config->window.width, config->window.height,
                                  config->window.title);
  if (app.window.ctx == NULL) {
    *res = -1;
    return (ClaymoreApp){0};
  }

  return app;
}
