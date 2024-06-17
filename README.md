# Claymore
A C Game Engine

> ⚠️ The Project as well as the Documentation is not yet finished and is prone to change.

## Libraries

- [RGFW](https://github.com/ColleagueRiley/RGFW): cross platform lightweight single-header very simple-to-use window library.
- [CGLM](https://github.com/recp/cglm): Highly Optimized 2D/3D Graphics Math for C.
- [STB](https://github.com/nothings/stb): stb single-file public domain libraries for C/C++. I use `stb_image.h` and `stb_truetype.h`.
- [Miniaudio](https://github.com/mackron/miniaudio): Audio playback and capture library written in C, in a single source file.
- [glad](https://github.com/Dav1dde/glad): Multi-Language GL Loader-Generator.
- [cebus](https://github.com/Code-Nycticebus/cebus): My own core C library.

## Building Claymore
First clone the repo:

```terminal
git clone --recursive https://github.com/Code-Nycticebus/claymore
```

Build or use it with [pybuildc](https://github.com/Code-Nycticebus/pybuildc):

```terminal
pybuildc -d claymore build
```


or build it with a c compiler:
```terminal
gcc build/claymore.c
./a.out
```

## Running Sandbox

Build or run with [pybuildc](https://github.com/Code-Nycticebus/pybuildc):

```terminal
pybuildc -d sandbox run
```

or build it with a c compiler:
```terminal
gcc build/sandbox.c
./a.out
```

Change into the `sandbox` directory and run the executable

```terminal
cd sandbox
./sandbox
```


## Usage
Check out the [documentation](https://github.com/Code-Nycticebus/claymore/blob/main/docs/00-start.md) for more info. 
For an example project, take a look at [shadertoy](https://github.com/Code-Nycticebus/shadertoy).

