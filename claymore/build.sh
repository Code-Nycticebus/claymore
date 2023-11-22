#!/bin/bash
# Generated by pybuildc

#----------Variables-----------#
PROJECT="claymore"
CC="clang"
BUILD_DIR=".build"
CFLAGS=()

#-------------Setup------------#
SCRIPT_DIR=$(dirname "$0")
set -xe
cd $SCRIPT_DIR

BIN=$BUILD_DIR/bin/$PROJECT
mkdir -p $BUILD_DIR/bin
mkdir -p $BUILD_DIR/obj

#---------Dependencies---------#
bash deps/stb_image/build.sh
bash deps/stb_truetype/build.sh
bash deps/miniaudio/build.sh

#---------Compilation---------#
mkdir -p $BUILD_DIR/obj/claymore/core
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/core/app.c -o $BUILD_DIR/obj/claymore/core/app.o -c
mkdir -p $BUILD_DIR/obj/claymore/core
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/core/camera.c -o $BUILD_DIR/obj/claymore/core/camera.o -c
mkdir -p $BUILD_DIR/obj/claymore/core
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/core/model.c -o $BUILD_DIR/obj/claymore/core/model.o -c
mkdir -p $BUILD_DIR/obj/claymore/core
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/core/scene.c -o $BUILD_DIR/obj/claymore/core/scene.o -c
mkdir -p $BUILD_DIR/obj/claymore/core
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/core/window.c -o $BUILD_DIR/obj/claymore/core/window.o -c
mkdir -p $BUILD_DIR/obj/claymore/debug
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/debug/debug.c -o $BUILD_DIR/obj/claymore/debug/debug.o -c
mkdir -p $BUILD_DIR/obj/claymore/events
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/events/event.c -o $BUILD_DIR/obj/claymore/events/event.o -c
mkdir -p $BUILD_DIR/obj/claymore/events
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/events/key.c -o $BUILD_DIR/obj/claymore/events/key.o -c
mkdir -p $BUILD_DIR/obj/claymore/events
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/events/mouse.c -o $BUILD_DIR/obj/claymore/events/mouse.o -c
mkdir -p $BUILD_DIR/obj/claymore/logger
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/logger/logger.c -o $BUILD_DIR/obj/claymore/logger/logger.o -c
mkdir -p $BUILD_DIR/obj/claymore/renderer
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/renderer/context.c -o $BUILD_DIR/obj/claymore/renderer/context.o -c
mkdir -p $BUILD_DIR/obj/claymore/renderer
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/renderer/font.c -o $BUILD_DIR/obj/claymore/renderer/font.o -c
mkdir -p $BUILD_DIR/obj/claymore/renderer
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/renderer/mesh.c -o $BUILD_DIR/obj/claymore/renderer/mesh.o -c
mkdir -p $BUILD_DIR/obj/claymore/renderer
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/renderer/render_buffer.c -o $BUILD_DIR/obj/claymore/renderer/render_buffer.o -c
mkdir -p $BUILD_DIR/obj/claymore/renderer
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/renderer/render_command.c -o $BUILD_DIR/obj/claymore/renderer/render_command.o -c
mkdir -p $BUILD_DIR/obj/claymore/renderer
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/renderer/renderer2D.c -o $BUILD_DIR/obj/claymore/renderer/renderer2D.o -c
mkdir -p $BUILD_DIR/obj/claymore/renderer
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/renderer/renderer_lines.c -o $BUILD_DIR/obj/claymore/renderer/renderer_lines.o -c
mkdir -p $BUILD_DIR/obj/claymore/renderer
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/renderer/shader.c -o $BUILD_DIR/obj/claymore/renderer/shader.o -c
mkdir -p $BUILD_DIR/obj/claymore/renderer
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/renderer/textures.c -o $BUILD_DIR/obj/claymore/renderer/textures.o -c
mkdir -p $BUILD_DIR/obj/claymore/sound
$CC -Ideps/stb_image/include -Ideps/stb_image/src -Ideps/stb_truetype/include -Ideps/stb_truetype/src -Ideps/miniaudio/include -Ideps/miniaudio/src -Ideps/glew/glew-linux/glew/include -Ideps/glfw/glfw-linux/glfw/include -Ideps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=10 ${CFLAGS[@]} src/claymore/sound/sound.c -o $BUILD_DIR/obj/claymore/sound/sound.o -c
ar rcs $BUILD_DIR/bin/libclaymore.a $BUILD_DIR/obj/claymore/core/app.o $BUILD_DIR/obj/claymore/core/camera.o $BUILD_DIR/obj/claymore/core/model.o $BUILD_DIR/obj/claymore/core/scene.o $BUILD_DIR/obj/claymore/core/window.o $BUILD_DIR/obj/claymore/debug/debug.o $BUILD_DIR/obj/claymore/events/event.o $BUILD_DIR/obj/claymore/events/key.o $BUILD_DIR/obj/claymore/events/mouse.o $BUILD_DIR/obj/claymore/logger/logger.o $BUILD_DIR/obj/claymore/renderer/context.o $BUILD_DIR/obj/claymore/renderer/font.o $BUILD_DIR/obj/claymore/renderer/mesh.o $BUILD_DIR/obj/claymore/renderer/render_buffer.o $BUILD_DIR/obj/claymore/renderer/render_command.o $BUILD_DIR/obj/claymore/renderer/renderer2D.o $BUILD_DIR/obj/claymore/renderer/renderer_lines.o $BUILD_DIR/obj/claymore/renderer/shader.o $BUILD_DIR/obj/claymore/renderer/textures.o $BUILD_DIR/obj/claymore/sound/sound.o