#include "cm.h"
#include <stdio.h>

#include "GLFW/glfw3.h"
#define _CM_RENDERER_INTERNAL
#include "claymore/renderer/renderer2D.h"
#define _CM_APP_INTERNAL
#include "claymore/core/app.h"
#include "window.h"

int main(void) {
  ClaymoreConfig config = cm_app_config();
  ClaymoreApp app = claymore_app_init(&config);
  cm_renderer_init(&app.renderer);

  cm_app_init(&app);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(app.window.ctx)) {
    cm_window_input_handler(app.window.ctx);

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    cm_app_update(&app);
    cm_renderer_draw(&app.renderer);

    /* Swap front and back buffers */
    glfwSwapBuffers(app.window.ctx);

    /* Poll for and process events */
    glfwPollEvents();
  }

  cm_app_free();

  glfwTerminate();
  return 0;
}
