#ifndef __CLAYMORE_ENTRYPOINT__
#define __CLAYMORE_ENTRYPOINT__

#define CLAYMORE_APP_INTERNAL
#include "claymore/app/app.h"

const ClaymoreConfig *claymore_init(void);

int main(void) {
  const ClaymoreConfig *config = claymore_init();

  app_internal_init(config);

  while (app_internal_update()) {
    // App running!
  }

  app_internal_terminate();
}

#else
#error "claymore entrypoint included twice"
#endif
