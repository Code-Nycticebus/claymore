sudo apt install libglu1-mesa-dev

mkdir glew-linux
cd glew-linux

git clone https://github.com/nigels-com/glew.git

make -C ./glew/auto/ PYTHON=python3
make -C ./glew/ glew.lib.static

mv glew/lib ./




