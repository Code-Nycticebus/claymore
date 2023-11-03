@echo off
cd cglm
echo building clgm
call build_win32.bat
cd ..

cd glew
echo building glew
python build_win32.py
cd ..

cd glfw
echo building glfw
python build_win32.py
cd ..
