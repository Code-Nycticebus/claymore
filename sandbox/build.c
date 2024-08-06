#define CM_DIR "../claymore/"
#include "../claymore/build/claymore.c"

int main(int argc, const char **argv) {
  (void)argc;
  Arena arena = {0};
  Path exe = path_name(str_from_cstr(argv[0]));
  claymore_project_build(exe, PATH("."), /* rebuild = */ true);
  Cmd cmd = cmd_new(&arena);
  cmd_push(&cmd, str_format(&arena, "./" STR_FMT, STR_ARG(exe)));
  cmd_exec_da(ErrPanic, &cmd);
  arena_free(&arena);
}
