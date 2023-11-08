PROJECT="claymore"
CC="clang"
BUILD_DIR=".build/debug"
BIN_DIR="$BUILD_DIR/bin"
BIN="$BIN_DIR/$PROJECT"
OBJ_PATH="$BUILD_DIR/obj"
OBJ="$OBJ_PATH/$PROJECT"
SCRIPT_DIR=$(dirname "$0")

set -xe

cd $SCRIPT_DIR

sh deps/stb_image/build.sh
sh deps/stb_truetype/build.sh
sh deps/miniaudio/build.sh

mkdir -p $BIN_DIR
mkdir -p $OBJ_PATH

$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/renderer/context.c -o .build/debug/obj/claymore/renderer/context.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/renderer/textures.c -o .build/debug/obj/claymore/renderer/textures.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/renderer/renderer2D.c -o .build/debug/obj/claymore/renderer/renderer2D.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/renderer/mesh.c -o .build/debug/obj/claymore/renderer/mesh.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/renderer/shader.c -o .build/debug/obj/claymore/renderer/shader.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/renderer/render_command.c -o .build/debug/obj/claymore/renderer/render_command.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/renderer/font.c -o .build/debug/obj/claymore/renderer/font.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/renderer/render_buffer.c -o .build/debug/obj/claymore/renderer/render_buffer.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/debug/debug.c -o .build/debug/obj/claymore/debug/debug.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/events/mouse.c -o .build/debug/obj/claymore/events/mouse.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/events/key.c -o .build/debug/obj/claymore/events/key.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/events/event.c -o .build/debug/obj/claymore/events/event.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/logger/logger.c -o .build/debug/obj/claymore/logger/logger.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/core/model.c -o .build/debug/obj/claymore/core/model.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/core/camera.c -o .build/debug/obj/claymore/core/camera.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/core/window.c -o .build/debug/obj/claymore/core/window.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/core/app.c -o .build/debug/obj/claymore/core/app.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/core/scene.c -o .build/debug/obj/claymore/core/scene.o -c
$CC -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -I../claymore/src -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 ../claymore/src/claymore/sound/sound.c -o .build/debug/obj/claymore/sound/sound.o -c
ar rcs .build/debug/bin/libclaymore.a .build/debug/obj/claymore/renderer/context.o .build/debug/obj/claymore/renderer/textures.o .build/debug/obj/claymore/renderer/renderer2D.o .build/debug/obj/claymore/renderer/mesh.o .build/debug/obj/claymore/renderer/shader.o .build/debug/obj/claymore/renderer/render_command.o .build/debug/obj/claymore/renderer/font.o .build/debug/obj/claymore/renderer/render_buffer.o .build/debug/obj/claymore/debug/debug.o .build/debug/obj/claymore/events/mouse.o .build/debug/obj/claymore/events/key.o .build/debug/obj/claymore/events/event.o .build/debug/obj/claymore/logger/logger.o .build/debug/obj/claymore/core/model.o .build/debug/obj/claymore/core/camera.o .build/debug/obj/claymore/core/window.o .build/debug/obj/claymore/core/app.o .build/debug/obj/claymore/core/scene.o .build/debug/obj/claymore/sound/sound.o
