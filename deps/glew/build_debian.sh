sudo apt-get install build-essential libxmu-dev libxi-dev libgl-dev freeglut3-dev

mkdir glew-linux
cd glew-linux

git clone https://github.com/nigels-com/glew.git

make -C ./glew/auto/
make -C ./glew/ glew.lib.static

mv glew/lib ./




