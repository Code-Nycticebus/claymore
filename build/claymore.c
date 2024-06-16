#ifndef __BUILD_CLAYMORE_H__
#define __BUILD_CLAYMORE_H__

#ifndef CC
#define CC "gcc"
#endif

#include "cebus.h"

#define SRC_DIR "claymore/src/"
#define LIB_DIR "claymore/libs/"

typedef DA(Str) Cmd;
typedef DA(Str) Paths;

const Str claymore_cflags[] = {
    STR_STATIC("-std=c99"),
    STR_STATIC("-I" SRC_DIR),
    STR_STATIC("-I" LIB_DIR "cebus"),
    STR_STATIC("-I" LIB_DIR "cglm/include"),
    STR_STATIC("-I" LIB_DIR "glad/src"),
    STR_STATIC("-I" LIB_DIR "RGFW/src"),
    STR_STATIC("-I" LIB_DIR "stb_image/src"),
    STR_STATIC("-I" LIB_DIR "stb_truetype/src"),
    STR_STATIC("-I" LIB_DIR "miniaudio/src"),
};

const Str claymore_lib_files[] = {
    STR_STATIC(LIB_DIR "RGFW/src/RGFW.c"),
    STR_STATIC(LIB_DIR "glad/src/glad.c"),
    STR_STATIC(LIB_DIR "miniaudio/src/miniaudio.c"),
    STR_STATIC(LIB_DIR "stb_image/src/stb_image.c"),
    STR_STATIC(LIB_DIR "stb_truetype/src/stb_truetype.c"),
    STR_STATIC("build/cebus.c"),
};

const Str claymore_files[] = {
    STR_STATIC(SRC_DIR "claymore/app/event.c"),
    STR_STATIC(SRC_DIR "claymore/app/camera.c"),
    STR_STATIC(SRC_DIR "claymore/app/scene.c"),
    STR_STATIC(SRC_DIR "claymore/app/app.c"),
    STR_STATIC(SRC_DIR "claymore/app/sound.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/gpu.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/context.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/2D/font.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/2D/quads.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/2D/renderer2D.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/2D/circles.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/2D/sprites.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/texture.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/shaders.c"),
    STR_STATIC(SRC_DIR "claymore/renderer/mesh.c"),

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

void compile_libs(Arena *arena, Paths *objs) {
  Arena scratch = {0};
  Cmd cmd = {0};
  da_init(&cmd, &scratch);

  for (usize i = 0; i < ARRAY_LEN(claymore_lib_files); ++i) {
    da_push(&cmd, STR(CC));
    da_push(&cmd, STR("-c"));
    da_extend(&cmd, ARRAY_LEN(claymore_cflags), claymore_cflags);

    da_push(&cmd, STR("-o"));
    Str name = claymore_lib_files[i];
    name = str_chop_right_by_delim(&name, '/');
    name = str_chop_by_delim(&name, '.');
    Str out = str_format(arena, "build/obj/" STR_FMT ".o", STR_ARG(name));
    da_push(objs, out);

    if (!file_exists(out)) {
      da_push(&cmd, out);
      da_push(&cmd, claymore_lib_files[i]);

      cebus_log_info("Building: libs");
      cmd_exec(ErrPanic, cmd.len, cmd.items);
    }
    da_clear(&cmd);
  }

  arena_free(&scratch);
}

void compile_claymore(void) {
  Arena arena = {0};
  Paths objs = {0};
  da_init(&objs, &arena);

  create_directory("build/obj");
  file_write_str(STR("build/obj/.gitignore"), STR("*\n"), ErrPanic);

  compile_libs(&arena, &objs);

  cebus_log_info("Building: build/lib/libclaymore.a");

  Cmd cmd = {0};
  da_init(&cmd, &arena);

  for (usize i = 0; i < ARRAY_LEN(claymore_files); ++i) {
    da_push(&cmd, STR(CC));
    da_push(&cmd, STR("-c"));
    da_extend(&cmd, ARRAY_LEN(claymore_cflags), claymore_cflags);

    da_push(&cmd, STR("-o"));
    Str name = claymore_files[i];
    name = str_chop_right_by_delim(&name, '/');
    name = str_chop_by_delim(&name, '.');
    Str out = str_format(&arena, "build/obj/" STR_FMT ".o", STR_ARG(name));
    da_push(&cmd, out);

    da_push(&objs, out);

    da_push(&cmd, claymore_files[i]);

    cmd_exec(ErrPanic, cmd.len, cmd.items);
    da_clear(&cmd);
  }

  create_directory("build/lib");
  file_write_str(STR("build/lib/.gitignore"), STR("*\n"), ErrPanic);

  da_push(&cmd, STR("ar"));
  da_push(&cmd, STR("rcs"));
  da_push(&cmd, STR("build/lib/libclaymore.a"));
  da_extend(&cmd, objs.len, objs.items);

  cmd_exec(ErrPanic, cmd.len, cmd.items);

  arena_free(&arena);
}

#ifndef CLAYMORE_INCLUDE
#define CEBUS_IMPLEMENTATION
#include "cebus.h"
int main(void) { compile_claymore(); }
#endif

#endif /* !__BUILD_CLAYMORE_H__ */
