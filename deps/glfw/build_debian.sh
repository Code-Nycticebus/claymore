sudo apt install xorg-dev

mkdir glfw-linux
cd glfw-linux

git clone https://github.com/glfw/glfw.git

cmake -S ./glfw -B ./build
make -C ./build
