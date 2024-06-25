#ifndef CC
#define CC "gcc"
#endif

#define CM_DIR "../claymore/" // Relative to working directory!
#include "../claymore/build/claymore.h"

int main(void) {
  compile_claymore(false);

  Arena arena = {0};

  Paths files = da_new(&arena);
  cmd_push(&files, STR_STATIC("src/utils/fps.c"));
  cmd_push(&files, STR_STATIC("src/utils/menu.c"));
  cmd_push(&files, STR_STATIC("src/scenes/hello.c"));
  cmd_push(&files, STR_STATIC("src/scenes/benchmark-2d.c"));
  cmd_push(&files, STR_STATIC("src/scenes/test.c"));

  Cmd cflags = cmd_new(&arena);
  cmd_push(&cflags, STR_STATIC("-Isrc"));

  compile_file(STR("src/sandbox.c"), &files, &cflags);

  arena_free(&arena);
}
