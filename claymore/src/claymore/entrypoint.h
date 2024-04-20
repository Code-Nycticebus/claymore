#ifdef __CLAYMORE_ENTRYPOINT__
#error "claymore entrypoint included twice"
#else
#define __CLAYMORE_ENTRYPOINT__

#include "claymore.h"

#if defined(WINDOWS) && !defined(CLAYMORE_DEBUG)
#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#define main(...)                                                              \
  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,                 \
                 LPSTR lpCmdLine, int nCmdShow)
#endif

// User defined
ClaymoreConfig *claymore_init(void);

int main(void) {
  ClaymoreConfig *config = claymore_init();
  if (!cm_app_internal_init(config)) {
    return -1;
  }
  while (cm_app_internal_update()) {
    // App running!
  }
  cm_app_internal_final();
}

#endif
