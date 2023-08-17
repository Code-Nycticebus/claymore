sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev freeglut3-dev
make -C ./glew/auto/
make -C ./glew/ glew.lib.static GLEW_DEST=glew-linux


