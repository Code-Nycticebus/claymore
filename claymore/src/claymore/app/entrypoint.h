#ifndef __CLAYMORE_ENTRYPOINT__
#define __CLAYMORE_ENTRYPOINT__

#include "claymore/defines.h" // IWYU pragma: export

#include "GL/glew.h"

#include "renderer/context.h"
#include "window.h"

typedef struct {
  struct {
    const char *title;
    usize width;
    usize height;
  } window;
} ClaymoreConfig;

ClaymoreConfig claymore_init(void);

int main(void) {
  ClaymoreConfig config = claymore_init();

  if (!cm_window_create(config.window.width, config.window.height,
                        config.window.title)) {
    clib_log_error("Could not open window");
    return -1;
  }

  cm_platform_context_init(cm_window_context());

  glClearColor(1, 0, 0, 1);
  while (!cm_window_should_close()) {
    glClear(GL_COLOR_BUFFER_BIT);

    cm_window_swap_buffers();
    cm_window_poll_events();
  }

  cm_window_close();
}

#else
#error "Entrypoint included twice"
#endif
