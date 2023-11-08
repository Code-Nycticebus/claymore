PROJECT="sandbox"
CC="clang"
BUILD_DIR=".build/debug"
BIN_DIR="$BUILD_DIR/bin"
BIN="$BIN_DIR/$PROJECT"
OBJ_PATH="$BUILD_DIR/obj"
OBJ="$OBJ_PATH/$PROJECT"
SCRIPT_DIR=$(dirname "$0")

set -xe

cd $SCRIPT_DIR

sh ../claymore/build.sh

mkdir -p $BIN_DIR
mkdir -p $OBJ_PATH

$CC -Iinclude -I../claymore/src -Iinclude -I../claymore/deps/stb_image/src -Iinclude -I../claymore/deps/stb_truetype/src -Iinclude -I../claymore/deps/miniaudio/src -I../claymore/deps/glew/glew-linux/glew/include -I../claymore/deps/glfw/glfw-linux/glfw/include -I../claymore/deps/cglm/cglm-linux/cglm/include -Isrc -g -Wall -Wextra -Werror -Wpedantic -Wshadow -Wnull-dereference -Wformat=2 -D_CRT_SECURE_NO_WARNINGS -std=c11 -DCM_SCENES_MAX=8 src/fps.c src/sandbox.c src/ortho/ortho_scene.c src/ortho/background.c src/ortho/ortho_layer.c src/menu/menu_scene.c src/models/camera.c src/models/models.c src/gui/gui.c src/instancing/instancing.c src/particles/particle.c src/texture/texture.c -o $BIN -L../claymore/.build/debug/bin -lclaymore -L../claymore/deps/stb_image/.build/release/bin -lstb_image -L../claymore/deps/stb_truetype/.build/release/bin -lstb_truetype -lm -L../claymore/deps/miniaudio/.build/release/bin -lminiaudio -ldl -lpthread -lm -lGL -L../claymore/deps/glew/glew-linux/lib -lGLEW -L../claymore/deps/glfw/glfw-linux/build/src -lglfw3 -L../claymore/deps/cglm/cglm-linux/build -lcglm -lm
