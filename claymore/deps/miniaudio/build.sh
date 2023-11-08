PROJECT="miniaudio"
CC="clang"
BUILD_DIR=".build/release"
BIN_DIR="$BUILD_DIR/bin"
BIN="$BIN_DIR/$PROJECT"
OBJ_PATH="$BUILD_DIR/obj"
OBJ="$OBJ_PATH/$PROJECT"
SCRIPT_DIR=$(dirname "$0")

set -xe

cd $SCRIPT_DIR

mkdir -p $BIN_DIR
mkdir -p $OBJ_PATH

$CC -I../claymore/deps/miniaudio/src -Wall -Wpedantic -O2 -DNDEBUG src/miniaudio.c -o $OBJ.o -c
ar rcs .build/release/bin/libminiaudio.a $OBJ.o
