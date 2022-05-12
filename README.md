# Installation Notes
This project was built using Ubuntu on WSL, using VS Code Server. Makefiles were manually written, although there exists a [Cmake option for BGFX](https://github.com/widberg/bgfx.cmake). It was run through [MobaXTerm](https://mobaxterm.mobatek.net/demo.html) terminal connecting to WSL, as graphical applications are not supported out of the box with WSL currently. MobaXTerm facilitates forwarding of the WSL DISPLAY environment variable to Windows desktop.

Linux setup used from [bgfx build setup](https://bkaradzic.github.io/bgfx/build.html), with additional SDL2 library:

```
sudo apt-get install libsdl2-dev libgl1-mesa-dev x11proto-core-dev libx11-dev
```

Installed bgfx libs:
```
git clone https://github.com/bkaradzic/bx.git
git clone https://github.com/bkaradzic/bimg.git
git clone https://github.com/bkaradzic/bgfx.git
```

Build command for bgfx on Linux:
```
cd bgfx
make linux
```

I used `sudo apt-get install libsdl2-dev` and not `libsdl2-2.0` like the [bgfx start tutorial](https://www.sandeepnambiar.com/getting-started-with-bgfx/) suggested.

# Resources Referenced
https://gist.github.com/dirkk0/cad259e6a3965abb4178

# VS Code
If you get SDL2 squiggly errors due to begin_code.h being missing, follow the steps here to add SDL2 to your path:
https://stackoverflow.com/a/69476174/19099501