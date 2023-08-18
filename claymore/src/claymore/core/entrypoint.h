#if defined(_CM_ENTRYPOINT_H__)
#error Entrypoint was included twice
#endif

#define _CM_ENTRYPOINT_H__

#include "claymore/core/app.h"

int main(void) {
  const ClaymoreConfig config = claymore_config();

  CmApp app = {0};

  if (!cm_app_init(&app, &config)) {
    return -1;
  }

  cm_app_run(&app, &config);

  cm_app_shutdown(&app);

  cm_window_shutdown();
  return 0;
}
