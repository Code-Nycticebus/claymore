#if defined(_CM_ENTRYPOINT_H__)
#error Entrypoint was included twice
#endif

#define _CM_ENTRYPOINT_H__

#include "claymore/core/app.h"

// Turns of console in release build on windows
#if defined(_WIN32) && !defined(_CM_DEBUG)
#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#define main(...)                                                              \
  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,                 \
                 LPSTR lpCmdLine, int nCmdShow)
#endif

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
