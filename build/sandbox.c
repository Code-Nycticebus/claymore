#define CLAYMORE_INCLUDE
#include "claymore.c"

#define CEBUS_IMPLEMENTATION
#include "cebus.h"

#ifndef CC
#define CC "gcc"
#endif

const Str files[] = {
    STR_STATIC("sandbox/src/utils/fps.c"),
    STR_STATIC("sandbox/src/hello.c"),
    STR_STATIC("sandbox/src/benchmark-2d.c"),
    STR_STATIC("sandbox/src/test.c"),
};

const Str cflags[] = {
    STR("-Iclaymore/src"),
    STR("-Isandbox/src"),
};

void compile_file(Str filename) {
  cebus_log_info("Building: " STR_FMT, STR_ARG(filename));
  Arena arena = {0};
  Cmd cmd = cmd_new(&arena);

  cmd_push(&cmd, STR(CC));

  Str out = str_copy(filename, &arena);
  out = str_chop_right_by_delim(&out, '/');
  out = str_chop_by_delim(&out, '.');
  out = str_append(STR("sandbox/"), out, &arena);
  cmd_push(&cmd, STR("-o"), out);

  cmd_extend(&cmd, cflags);
  cmd_extend(&cmd, claymore_cflags);

  cmd_push(&cmd, filename);
  cmd_extend(&cmd, files);

  cmd_push(&cmd, STR("-Lbuild/lib"), STR("-lclaymore"));
  cmd_extend(&cmd, claymore_libs);

  cmd_exec_da(ErrPanic, &cmd);

  arena_free(&arena);
}

int main(void) {
  compile_claymore();
  compile_file(STR("sandbox/src/sandbox.c"));
}
