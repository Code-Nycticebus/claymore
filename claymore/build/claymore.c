#if defined(LINUX)
#include <sys/stat.h>
#elif defined(WINDOWS)
#include <windows.h>
#endif

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#endif
#define CEBUS_IMPLEMENTATION
#include "cebus.h" // IWYU pragma: keep
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wall"
#endif

#ifndef CC
#define CC "gcc"
#endif

#ifndef CM_DIR
#define CM_DIR "./"
#endif

#define CM_SRC_DIR CM_DIR "src/"
#define CM_LIB_DIR CM_DIR "libs/"

#define CM_OUT_DIR CM_DIR "build/"
#define CM_DUMP_DIR CM_OUT_DIR ".dump/"

#define CM_OUTFILE CM_OUT_DIR "libclaymore.a"

void create_directory(const char *path) {
#if defined(LINUX)
  mkdir(path, 0777); // NOLINT
#elif defined(WINDOWS)
  CreateDirectory(path, NULL);
#endif
}

static void claymore_collect_libs(Cmd *objs) {
  const Path libs[] = {
      PATH(CM_LIB_DIR "RGFW/src/RGFW.c"),
      PATH(CM_LIB_DIR "glad/src/glad.c"),
      PATH(CM_LIB_DIR "miniaudio/src/miniaudio.c"),
      PATH(CM_LIB_DIR "stb_image/src/stb_image.c"),
      PATH(CM_LIB_DIR "stb_truetype/src/stb_truetype.c"),
      PATH(CM_DUMP_DIR "cebus.c"),
  };

  Str content = STR("#define CEBUS_IMPLEMENTATION\n#include \"../cebus.h\"\n");
  fs_file_write_str(STR(CM_DUMP_DIR "/cebus.c"), content, ErrPanic);

  Arena scratch = {0};
  Cmd cmd = cmd_new(&scratch);

  for (usize i = 0; i < ARRAY_LEN(libs); ++i) {
    Str name = path_stem(libs[i]);
    Str out = str_format(objs->arena, CM_DUMP_DIR STR_FMT ".o", STR_ARG(name));
    da_push(objs, out);

    if (!fs_exists(out)) {
      cmd_push(&cmd, STR(CC), STR("-c"));
      cmd_push(&cmd, STR("-O2"), STR("-DNDEBUG"), STR("-fPIC"));
      cmd_push(&cmd, STR("-o"), out);
      cmd_push(&cmd, libs[i]);

      cebus_log_info("Building: " STR_FMT, STR_ARG(out));
      cmd_exec_da(ErrPanic, &cmd);
    }
    da_clear(&cmd);
  }

  arena_free(&scratch);
}

void claymore_collect_cflags(Cmd *cmd) {
  const Str claymore_cflags[] = {
      STR_STATIC("-std=c99"),
      STR_STATIC("-I" CM_SRC_DIR),
      STR_STATIC("-I" CM_LIB_DIR "cebus"),
      STR_STATIC("-I" CM_LIB_DIR "cglm/include"),
      STR_STATIC("-I" CM_LIB_DIR "glad/src"),
      STR_STATIC("-I" CM_LIB_DIR "RGFW/src"),
      STR_STATIC("-I" CM_LIB_DIR "stb_image/src"),
      STR_STATIC("-I" CM_LIB_DIR "stb_truetype/src"),
      STR_STATIC("-I" CM_LIB_DIR "miniaudio/src"),
  };
  cmd_extend(cmd, claymore_cflags);
}

void claymore_collect_links(Cmd *cmd) {
#if defined(LINUX)
  const Str links[] = {
      STR_STATIC("-lm"),
      STR_STATIC("-lGL"),
      STR_STATIC("-lX11"),
      STR_STATIC("-lXrandr"),
  };
#elif defined(WINDOWS)
  const Str links[] = {
      STR_STATIC("-lopengl32"), STR_STATIC("-lShell32"), STR_STATIC("-lUser32"),
      STR_STATIC("-lGdi32"),    STR_STATIC("-lwinmm"),
  };
#endif
  cmd_extend(cmd, links);
}

void claymore_collect_files(Cmd *cmd) {
  Arena scratch = {0};
  Cmd sub = cmd_new(&scratch);
  FsIter it = fs_iter_begin(PATH(CM_SRC_DIR), true);
  while (fs_iter_next_suffix(&it, STR(".c"))) {
    cmd_push(&sub, STR(CC), STR("-c"), STR("-fPIC"));
    claymore_collect_cflags(&sub);

    Str stem = path_stem(it.current.path);
    Path path = str_format(cmd->arena, "%s/" STR_FMT ".o", CM_DUMP_DIR, STR_ARG(stem));
    cmd_push(&sub, STR("-o"), path);

    cmd_push(&sub, it.current.path);

    cmd_exec_da(ErrPanic, &sub);
    da_clear(&sub);

    da_push(cmd, path);
  }
  fs_iter_end(&it, ErrPanic);
  arena_free(&scratch);
}

void claymore_build(bool rebuild) {
  if (rebuild == false && fs_exists(PATH(CM_OUTFILE))) {
    return;
  }

  create_directory(CM_DUMP_DIR);
  fs_file_write_str(STR(CM_OUT_DIR ".gitignore"),
                    STR(".gitignore\n"
                        "libclaymore.a\n"
                        ".dump"),
                    ErrPanic);

  cebus_log_info("LIBRARY: " CM_OUTFILE);

  Arena arena = {0};
  Cmd cmd = cmd_new(&arena);

  cmd_push(&cmd, STR("ar"), STR("rcs"), STR(CM_OUTFILE));
  claymore_collect_libs(&cmd);
  claymore_collect_files(&cmd);

  cmd_exec_da(ErrPanic, &cmd);
}

void claymore_project_build(Path exe, Path directory, bool rebuild) {
  claymore_build(rebuild);

  Arena arena = {0};

  Path src_dir = path_new(&arena, directory, STR("src"));

  Cmd cmd = cmd_new(&arena);
  cmd_push(&cmd, STR(CC), STR("-Werror"), STR("-Wall"), STR("-Wextra"));

  Str outfile = path_new(&arena, directory, exe);

  cmd_push(&cmd, STR("-o"), outfile);

  cmd_push(&cmd, str_format(&arena, "-I" STR_FMT, STR_ARG(src_dir)));
  claymore_collect_cflags(&cmd);

  FsIter it = fs_iter_begin(src_dir, true);
  while (fs_iter_next_suffix(&it, STR(".c"))) {
    cmd_push(&cmd, str_copy(it.current.path, &arena));
  }
  fs_iter_end(&it, ErrPanic);

  cmd_push(&cmd, STR("-L" CM_OUT_DIR), STR("-lclaymore"));
  claymore_collect_links(&cmd);

  cebus_log_info("EXE: " STR_FMT, STR_ARG(outfile));
  Str content = str_join(STR(" "), cmd.len, cmd.items, &arena);
  cebus_log_debug(STR_FMT, STR_ARG(content));
  cmd_exec_da(ErrPanic, &cmd);
  arena_free(&arena);
}
