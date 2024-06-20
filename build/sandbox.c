#ifndef CC
#define CC "gcc"
#endif

#define CEBUS_IMPLEMENTATION
#include "cebus.h"

#define CLAYMORE_INCLUDE
#include "claymore.c"

const Str files[] = {
    STR_STATIC("sandbox/src/utils/fps.c"),
    STR_STATIC("sandbox/src/scenes/hello.c"),
    STR_STATIC("sandbox/src/scenes/benchmark-2d.c"),
    STR_STATIC("sandbox/src/scenes/test.c"),
    STR_STATIC("sandbox/src/scenes/template.c"),
};

const Str cflags[] = {
    STR("-I" CM_SRC_DIR),
    STR("-Isandbox/src"),
};

void compile_file(Str filename) {
  cebus_log_info("Building: " STR_FMT, STR_ARG(filename));
  Arena arena = {0};
  Cmd cmd = cmd_new(&arena);

  cmd_push(&cmd, STR(CC));

  Str name = filename;
  name = str_chop_right_by_delim(&name, '/');
  name = str_chop_by_delim(&name, '.');
  Str out = str_append(STR("sandbox/"), name, &arena);
  cmd_push(&cmd, STR("-o"), out);

  cmd_extend(&cmd, cflags);
  cmd_extend(&cmd, claymore_cflags);

  cmd_push(&cmd, filename);
  cmd_extend(&cmd, files);

  cmd_push(&cmd, STR("-L" CM_BUILD_DIR "/lib"), STR("-lclaymore"));
  cmd_extend(&cmd, claymore_libs);

  cmd_exec_da(ErrPanic, &cmd);

  arena_free(&arena);
}

int main(void) {
  compile_claymore();
  compile_file(STR("sandbox/src/sandbox.c"));
}
