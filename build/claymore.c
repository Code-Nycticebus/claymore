#ifndef __BUILD_CLAYMORE_H__
#define __BUILD_CLAYMORE_H__

#include "cebus.h"

#define SRC_DIR "claymore/src/"
#define LIB_DIR "claymore/libs/"

const Str claymore_cflags[] = {
    STR_STATIC("-std=c99"),
    STR_STATIC("-I" LIB_DIR "cebus"),
    STR_STATIC("-I" LIB_DIR "cglm/include"),
    STR_STATIC("-I" LIB_DIR "glad/src"),
    STR_STATIC("-I" LIB_DIR "RGFW/src"),
    STR_STATIC("-I" LIB_DIR "stb_image/src"),
    STR_STATIC("-I" LIB_DIR "stb_truetype/src"),
    STR_STATIC("-I" LIB_DIR "miniaudio/src"),
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

    // libs?
    STR_STATIC(LIB_DIR "RGFW/src/RGFW.c"),
    STR_STATIC(LIB_DIR "glad/src/glad.c"),
    STR_STATIC(LIB_DIR "miniaudio/src/miniaudio.c"),
    STR_STATIC(LIB_DIR "stb_image/src/stb_image.c"),
    STR_STATIC(LIB_DIR "stb_truetype/src/stb_truetype.c"),
    STR_STATIC("build/cebus.c"),
};

const Str claymore_libs[] = {
    STR_STATIC("-lm"),
    STR_STATIC("-lGL"),
    STR_STATIC("-lX11"),
    STR_STATIC("-lXrandr"),
};

#ifndef CLAYMORE_INCLUDE
int main(void) {}
#endif

#endif /* !__BUILD_CLAYMORE_H__ */
