# Installation Notes
This project was built using Ubuntu on WSL, using VS Code Server. Makefiles were manually written, although there exists a [Cmake option for BGFX](https://github.com/widberg/bgfx.cmake).

Linux setup used, from [bgfx build setup](https://bkaradzic.github.io/bgfx/build.html):

```
sudo apt-get install libgl1-mesa-dev x11proto-core-dev libx11-dev
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

