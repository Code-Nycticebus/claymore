# Claymore
A C Game Engine

> ⚠️ The Project as well as the Documentation is not yet finished and is prone to change.

## Libraries

- [RGFW](https://github.com/ColleagueRiley/RGFW): for window creation and input handling.
- [CGLM](https://github.com/recp/cglm): For linear algebra
- [STB](https://github.com/nothings/stb): I use `stb_image.h` and `stb_truetype.h` for image and font loading.
- [Miniaudio](https://github.com/mackron/miniaudio): For audio.
- [glad](https://github.com/Dav1dde/glad): For opengl function loading.
- [cebus](https://github.com/Code-Nycticebus/cebus): My own core library.

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
Check out the [documentation](https://github.com/Code-Nycticebus/claymore/blob/main/DOCUMENTATION.md) for more info. 
For an example project, take a look at [shadertoy](https://github.com/Code-Nycticebus/shadertoy).

