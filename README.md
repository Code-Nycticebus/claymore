# claymore
A C Game Engine

## Why claymore
Just don't. There are way better opengl frameworks out there. 
This is just a fun little project.

## Setup

Setup should just running these scripts.

### Build dependecies

#### Linux
```
cd deps
sh build_debian.sh
```

#### Windows
```
cd deps
.\build_win32.bat
```

### Build System
Claymore uses my own build system [pybuildc](https://github.com/Code-Nycticebus/pybuildc)

```
pybuildc -d sandbox run
```

*NOT IMPLEMENTED*
Or you can just run the build script that was generated by [pybuildc](https://github.com/Code-Nycticebus/pybuildc)

#### Linux
```
sh build.sh
```

#### Windows
```
.\build.bat
```