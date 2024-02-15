#ifndef __CLAYMORE_ENTRYPOINT__
#define __CLAYMORE_ENTRYPOINT__

#include "app.h"

const ClaymoreConfig *claymore_init(void);

int main(void) {
  const ClaymoreConfig *config = claymore_init();

  app_init(config);

  while (app_update()) {
    // App running!
  }

  app_terminate();
}

#else
#error "claymore entrypoint included twice"
#endif
