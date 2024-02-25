#ifdef __CLAYMORE_ENTRYPOINT__
#error "claymore entrypoint included twice"
#else
#define __CLAYMORE_ENTRYPOINT__

#include "defines.h"

#include "claymore/app/app.h"

#if defined(WINDOWS) && !defined(CLAYMORE_DEBUG)
#pragma comment(linker, "/subsystem:windows")
#include <windows.h>
#define main(...)                                                              \
  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,                 \
                 LPSTR lpCmdLine, int nCmdShow)
#endif

const ClaymoreConfig *claymore_init(void);

int main(void) {
  const ClaymoreConfig *config = claymore_init();
  app_internal_init(config);
  while (app_internal_update()) {
    // App running!
  }
  app_internal_final();
}

#endif
