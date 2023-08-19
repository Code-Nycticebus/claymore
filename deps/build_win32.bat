@echo off
cd cglm
call build_win32.bat
cd ..

cd glew
python build_win32.py
cd ..

cd glfw
python build_win32.py
cd ..
