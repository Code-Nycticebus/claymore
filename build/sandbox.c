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
  DA(Str) cmd = {0};
  da_init(&cmd, &arena);

  da_push(&cmd, STR(CC));
  da_push(&cmd, STR("-o"));

  Str out = str_copy(filename, &arena);
  out = str_chop_right_by_delim(&out, '/');
  out = str_chop_by_delim(&out, '.');
  da_push(&cmd, str_append(STR("sandbox/"), out, &arena));

  da_extend(&cmd, ARRAY_LEN(cflags), cflags);
  da_extend(&cmd, ARRAY_LEN(claymore_cflags), claymore_cflags);

  da_push(&cmd, filename);
  da_extend(&cmd, ARRAY_LEN(files), files);

  da_push(&cmd, STR("-Lbuild/lib"));
  da_push(&cmd, STR("-lclaymore"));
  da_extend(&cmd, ARRAY_LEN(claymore_libs), claymore_libs);

  cmd_exec(ErrPanic, cmd.len, cmd.items);
  arena_free(&arena);
}

int main(void) {
  compile_claymore();
  compile_file(STR("sandbox/src/sandbox.c"));
}
