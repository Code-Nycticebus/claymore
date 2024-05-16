sudo apt install xorg-dev libxkbcommon-x11-dev
sudo apt install libwayland-dev

mkdir glfw-linux
cd glfw-linux

git clone https://github.com/glfw/glfw.git

cmake -S ./glfw -B ./build
make -C ./build
