# Basic Roadmap
currently i'm not really working towards a goal so i decieded to create a roadmap.

## TODO
- [ ] Hot reloading
    - [ ] Could be a "scene" or i will create a exe
    - [ ] libraries need to be compiled with -fPIC

- [ ] Model loading
    - [ ] .obj files since they are easy to parse
    - [ ] .iqm for animations. raylib has info on it (don't forget to credit)

- [ ] Assets
    - [ ] A way to Embed assets
    - [ ] Asset manager?
    - [ ] Precompile shaders (https://www.khronos.org/opengl/wiki_opengl/index.php?title=Shader_Compilation#Binary_upload)

- [ ] Audio
    - [ ] Master, Music, Ambient options
    - [ ] Spatial audio
    - [ ] A way to embed /preload music

- [ ] Framebuffer
    - [ ] wrap the gl implementation
    - [ ] could be a scene 
    - [ ] something you load a scene into
    
- [ ] Dependencies
    - [x] Add cebus as submodule
    - [x] Add cglm as submodule
    - [x] Switch glfw to rgfw

- [ ] Render thread
    - [ ] render_update function
    - [ ] Lock mutex when regular updating. so rendering starts after scene update
    - [ ] Takes `const CmScene*`

- [ ] Misc
    - [ ] custom events/signals
    - [ ] implement font color
    - [ ] Make sandbox to a menu
    - [ ] GUI
    - [ ] Joystick support
    - [x] Overwork event system
    - [ ] README
        - [x] Credit all the libraries
        - [ ] Simple documentation
