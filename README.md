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

## Setup
First clone the repo:

```terminal
git clone https://github.com/Code-Nycticebus/claymore --recurse-submodules 
```

## Start a project
Claymore uses my own build system [pybuildc](https://github.com/Code-Nycticebus/pybuildc)

```
pybuildc new test-project
```

and add claymore as a dependency to the `pybuildc.toml`:

```toml
[libs.claymore]
dir="<CLAYMORE DIRECTORY>" 
type="pybuildc"
```

## Usage
Check out the [documentation](https://github.com/Code-Nycticebus/claymore/blob/main/DOC.md) for more info. 
For an example project, take a look at [shadertoy](https://github.com/Code-Nycticebus/shadertoy).

