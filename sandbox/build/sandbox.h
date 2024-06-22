#ifndef __SANDBOX_BUILD_H__
#define __SANDBOX_BUILD_H__

#ifndef CC
#define CC "gcc"
#endif

#include "cebus.h"

#define CM_DIR "../claymore/" // Relative to working directory!
#include "../../claymore/build/claymore.h"

const Str files[] = {
    STR_STATIC("src/utils/fps.c"),           //
    STR_STATIC("src/scenes/hello.c"),        //
    STR_STATIC("src/scenes/benchmark-2d.c"), //
    STR_STATIC("src/scenes/test.c"),         //
};

const Str cflags[] = {
    STR_STATIC("-I" CM_SRC_DIR),
    STR_STATIC("-Isrc"),
};

void compile_file(Str filename) {
  cebus_log_info("Building: " STR_FMT, STR_ARG(filename));
  Arena arena = {0};
  Cmd cmd = cmd_new(&arena);

  cmd_push(&cmd, STR(CC));

  Str name = filename;
  name = str_chop_right_by_delim(&name, '/');
  name = str_chop_by_delim(&name, '.');
  cmd_push(&cmd, STR("-o"), name);

  cmd_extend(&cmd, cflags);
  cmd_extend(&cmd, claymore_cflags);

  cmd_push(&cmd, filename);
  cmd_extend(&cmd, files);

  cmd_push(&cmd, STR("-L" CM_BUILD_DIR "/lib"), STR("-lclaymore"));
  cmd_extend(&cmd, claymore_libs);

  cmd_exec_da(ErrPanic, &cmd);

  arena_free(&arena);
}

#endif /* ifndef __SANDBOX_BUILD_H__ */
