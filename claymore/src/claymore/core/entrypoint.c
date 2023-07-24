#include "cm.h"

#include "GLFW/glfw3.h"
#define _CM_RENDERER_INTERNAL
#include "claymore/renderer/renderer2D.h"
#define _CM_APP_INTERNAL
#include "claymore/core/app.h"

#include "claymore/logger/logger.h"

int main(void) {

  ClaymoreConfig config = claymore_config();

  CmApp app = {0};
  if (!cm_app_init(&app, &config)) {
    return -1;
  }
  if (app.window == NULL) {
    return -1;
  }

  cm_renderer_init();

  claymore_init(&app);

  /* Loop until the user closes the window */
  while (app.run) {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    claymore_update(&app);

    cm_window_update(app.window);
  }

  claymore_free(&app);
  cm_renderer_shutdown();

  cm_app_shutdown(&app);

  cm_window_shutdown();
  return 0;
}
