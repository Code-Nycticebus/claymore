# Claymore
A C Game Engine

> ⚠️ The Project as well as the Documentation is WIP and is prone to change.

## Libraries

- [RGFW](https://github.com/ColleagueRiley/RGFW): cross platform lightweight single-header very simple-to-use window library.
- [CGLM](https://github.com/recp/cglm): Highly Optimized 2D/3D Graphics Math for C.
- [STB](https://github.com/nothings/stb): stb single-file public domain libraries for C/C++. I use `stb_image.h` and `stb_truetype.h`.
- [Miniaudio](https://github.com/mackron/miniaudio): Audio playback and capture library written in C, in a single source file.
- [glad](https://github.com/Dav1dde/glad): Multi-Language GL Loader-Generator.
- [cebus](https://github.com/Code-Nycticebus/cebus): My own core C library.

## Building Claymore
On linux you need to install these dependencies:
```terminal
sudo apt install libmesa-dev xorg-dev libxrandr-dev 
```

First clone and enter the repo:
```terminal
git clone --recursive https://github.com/Code-Nycticebus/claymore
cd claymore
```

Build it with a c compiler. From inside the `claymore` directory, compile the build system. you can even:
```terminal
cd claymore
gcc build/build.c && ./a.out
```

or build it with [pybuildc](https://github.com/Code-Nycticebus/pybuildc):
```terminal
pybuildc -d claymore build
```

## Running Sandbox
Build it with a c compiler. From inside of the `sandbox` directory:
```terminal
cd sandbox
gcc build.c && ./a.out
```

or build and run directly with [pybuildc](https://github.com/Code-Nycticebus/pybuildc):
```terminal
pybuildc -d sandbox run
```

## Usage
Check out the [documentation](https://github.com/Code-Nycticebus/claymore/blob/main/docs/00-start.md) for more info. 
For an example project, take a look at [shadertoy](https://github.com/Code-Nycticebus/shadertoy).

