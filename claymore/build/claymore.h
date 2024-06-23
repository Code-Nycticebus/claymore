#ifndef __BUILD_CLAYMORE_H__
#define __BUILD_CLAYMORE_H__

#ifndef CC
#define CC "gcc"
#endif

#define CEBUS_IMPLEMENTATION
#include "cebus.h"

#ifndef CM_DIR
#define CM_DIR "./"
#endif

#define CM_SRC_DIR CM_DIR "src/"
#define CM_LIB_DIR CM_DIR "libs/"
#define CM_BUILD_DIR CM_DIR "build/"

#define CM_OUT_DIR CM_BUILD_DIR "lib/"

// Output file
#define CM_OUTFILE CM_OUT_DIR "libclaymore.a"

typedef DA(Str) Paths;

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

const Str claymore_lib_files[] = {
    STR_STATIC(CM_LIB_DIR "RGFW/src/RGFW.c"),
    STR_STATIC(CM_LIB_DIR "glad/src/glad.c"),
    STR_STATIC(CM_LIB_DIR "miniaudio/src/miniaudio.c"),
    STR_STATIC(CM_LIB_DIR "stb_image/src/stb_image.c"),
    STR_STATIC(CM_LIB_DIR "stb_truetype/src/stb_truetype.c"),
    STR_STATIC(CM_BUILD_DIR "obj/cebus.c"),
};

const Str claymore_files[] = {
    STR_STATIC(CM_SRC_DIR "claymore/app/event.c"),
    STR_STATIC(CM_SRC_DIR "claymore/app/camera.c"),
    STR_STATIC(CM_SRC_DIR "claymore/app/scene.c"),
    STR_STATIC(CM_SRC_DIR "claymore/app/app.c"),
    STR_STATIC(CM_SRC_DIR "claymore/app/sound.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/gpu.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/context.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/2D/font.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/2D/quads.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/2D/renderer2D.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/2D/circles.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/2D/sprites.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/texture.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/shaders.c"),
    STR_STATIC(CM_SRC_DIR "claymore/renderer/mesh.c"),
};

#if defined(LINUX)
#include <sys/stat.h>
const Str claymore_libs[] = {
    STR_STATIC("-lm"),
    STR_STATIC("-lGL"),
    STR_STATIC("-lX11"),
    STR_STATIC("-lXrandr"),
};
#elif defined(WINDOWS)
#include <windows.h>
const Str claymore_libs[] = {
    STR_STATIC("-lopengl32"), STR_STATIC("-lShell32"), STR_STATIC("-lUser32"),
    STR_STATIC("-lGdi32"),    STR_STATIC("-lwinmm"),
};
#endif

void create_directory(const char *path) {
#if defined(LINUX)
  mkdir(path, 0777); // NOLINT
#elif defined(WINDOWS)
  CreateDirectory(path, NULL);
#endif
}

void create_cebus(void) {
  Str content = STR("#define CEBUS_IMPLEMENTATION\n"
                    "#include \"../cebus.h\"\n");
  file_write_str(STR(CM_BUILD_DIR "obj/cebus.c"), content, ErrPanic);
}

void compile_libs(Arena *arena, Paths *objs) {
  Arena scratch = {0};
  Cmd cmd = cmd_new(&scratch);

  create_cebus();

  for (usize i = 0; i < ARRAY_LEN(claymore_lib_files); ++i) {
    Str name = claymore_lib_files[i];
    name = str_chop_right_by_delim(&name, '/');
    name = str_chop_by_delim(&name, '.');
    Str out =
        str_format(arena, CM_BUILD_DIR "/obj/" STR_FMT ".o", STR_ARG(name));
    da_push(objs, out);

    if (!file_exists(out)) {
      cmd_push(&cmd, STR(CC), STR("-c"));
      cmd_push(&cmd, STR("-O2"), STR("-DNDEBUG"), STR("-fPIC"));
      cmd_extend(&cmd, claymore_cflags);

      cmd_push(&cmd, STR("-o"), out);

      cmd_push(&cmd, claymore_lib_files[i]);

      cebus_log_info("Building: " STR_FMT, STR_ARG(out));
      cmd_exec_da(ErrPanic, &cmd);
    }
    da_clear(&cmd);
  }

  arena_free(&scratch);
}

void compile_claymore(void) {
  Arena arena = {0};
  Paths objs = da_new(&arena);

  create_directory(CM_BUILD_DIR "obj");
  file_write_str(STR(CM_BUILD_DIR "obj/.gitignore"), STR("*\n"), ErrPanic);

  compile_libs(&arena, &objs);

  cebus_log_info("Building: " CM_OUTFILE);

  Cmd cmd = cmd_new(&arena);

  for (usize i = 0; i < ARRAY_LEN(claymore_files); ++i) {
    cmd_push(&cmd, STR(CC), STR("-c"), STR("-fPIC"));
    cmd_extend(&cmd, claymore_cflags);

    Str name = claymore_files[i];
    name = str_chop_right_by_delim(&name, '/');
    name = str_chop_by_delim(&name, '.');
    Str out =
        str_format(&arena, CM_BUILD_DIR "obj/" STR_FMT ".o", STR_ARG(name));
    cmd_push(&cmd, STR("-o"), out);

    da_push(&objs, out);

    cmd_push(&cmd, claymore_files[i]);

    cmd_exec_da(ErrPanic, &cmd);
    da_clear(&cmd);
  }

  create_directory(CM_BUILD_DIR "lib");
  file_write_str(STR(CM_BUILD_DIR "lib/.gitignore"), STR("*\n"), ErrPanic);

  cmd_push(&cmd, STR("ar"), STR("rcs"), STR(CM_OUTFILE));

  cmd_extend_da(&cmd, &objs);

  cmd_exec_da(ErrPanic, &cmd);

  arena_free(&arena);
}

void compile_file(Str filename, Paths *files, Cmd *cflags) {
  cebus_log_info("Building: " STR_FMT, STR_ARG(filename));
  Arena arena = {0};
  Cmd cmd = cmd_new(&arena);

  cmd_push(&cmd, STR(CC));

  Str name = filename;
  name = str_chop_right_by_delim(&name, '/');
  name = str_chop_by_delim(&name, '.');
  cmd_push(&cmd, STR("-o"), name);

  cmd_extend_da(&cmd, cflags);
  cmd_push(&cmd, STR_STATIC("-I" CM_SRC_DIR));
  cmd_extend(&cmd, claymore_cflags);

  cmd_push(&cmd, filename);
  cmd_extend_da(&cmd, files);

  cmd_push(&cmd, STR("-L" CM_BUILD_DIR "/lib"), STR("-lclaymore"));
  cmd_extend(&cmd, claymore_libs);

  cmd_exec_da(ErrPanic, &cmd);

  arena_free(&arena);
}

#endif /* !__BUILD_CLAYMORE_H__ */
