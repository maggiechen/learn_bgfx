# Purpose
A lightweight geometry parser w/ WASD navigation + Blender-type numpad shortcuts for shader experiments.

# Linux Setup
Built using Ubuntu on WSL, using VS Code Server. Makefiles were manually written, although there exists a [Cmake option for BGFX](https://github.com/widberg/bgfx.cmake). 

Run through [MobaXTerm](https://mobaxterm.mobatek.net/demo.html) terminal connecting to WSL, as graphical applications are not supported out of the box with WSL currently. MobaXTerm facilitates forwarding of the WSL DISPLAY environment variable to Windows desktop. 

Run in WSL:
```bash
sudo apt install ubuntu-desktop mesa-utils
export DISPLAY=localhost:0
export LIBGL_ALWAYS_INDIRECT=0
glxgears
```
And check that the gears turn at a decently quick pace. 

## Running examples
Watch: https://www.youtube.com/watch?v=oUQJm755GwY

### Recommended MobaXTerm settings
**Disable remote access to prevent malicious clients from connecting.** I received connection requests from unknown IP addresses during development, and a quick search showed the source was a malicious client that had been spamming random victims' ports over the past week.

**Turn on hardware acceleration.**

![](mobaxterm_recommended_settings.png)

Linux setup used from [bgfx build setup](https://bkaradzic.github.io/bgfx/build.html), with additional SDL2 library:

```bash
sudo apt-get install libsdl2-dev libgl1-mesa-dev x11proto-core-dev libx11-dev
```
I used `sudo apt-get install libsdl2-dev` and not `libsdl2-2.0` like the [bgfx start tutorial](https://www.sandeepnambiar.com/getting-started-with-bgfx/) suggested.

Installed bgfx libs:
```bash
git clone https://github.com/bkaradzic/bx.git
git clone https://github.com/bkaradzic/bimg.git
git clone https://github.com/bkaradzic/bgfx.git
```

Build command for bgfx on Linux:
```
cd bgfx
make projgen   # creates the projects for the bgfx libraries for all platforms
make linux   # build for linux, release AND debug
```
### VS Code
If you get SDL2 squiggly errors due to begin_code.h being missing, follow the steps here to add SDL2 to your path:
https://stackoverflow.com/a/69476174/19099501

### Resources Referenced
SDL initialization example: https://gist.github.com/dirkk0/cad259e6a3965abb4178
Had to do a bunch of fiddling just to get OpenGL working with WSL: https://github.com/microsoft/WSL/issues/2855#issuecomment-358861903

### Making bgfx examples
```bash
cd bgfx
make projgen # make project configs for everything..
make dist-linux # creates shaderc
cd examples
make all
```

### Debug build and run:
`sudo make all ARGS="debug" && /bin/main`
### Release build and run:
`sudo make all && /bin/main`

# Windows Visual Studio Setup
TODO: This set up will be overwritten if you go through it again. Write a version that puts the learn_bgfx project into another solution.

Create VS projects for examples and tools (including shaderc). 

> `.\bx\tools\bin\windows\genie --with-tools --with-examples vs2017`  
> `start .build\projects\vs2017\bgfx.sln`

Build shaderc project.

Create new project. Add C/C++/Preprocessor > Preprocessor Definitions:
```
BX_CONFIG_DEBUG=1
```
C/C++ > All Options > Additional Include Directories = `C:\dev\learn_bgfx\bx\include\compat\msvc;C:\dev\learn_bgfx\bimg\include;C:\dev\learn_bgfx\bgfx\include;C:\dev\learn_bgfx\bgfx\3rdparty;C:\dev\learn_bgfx\bx\include;%(AdditionalIncludeDirectories)`

Set: `C/C++ > Code Generation > Runtime Library = Multi-threaded Debug (/MTd)`. This is the option that bgfx projects are compiled with and if you don't use it in your project it will fail to compile.

Set Debugging > Working Directory = `<the folder where your compiled shaders reside>`, in this case this current folder that the Makefile is in. 

> **NOTE:**
> BGFX example project compiled shaders go:
> `C:\Dev\learn_bgfx\bgfx\examples\runtime\shaders`

Then run 
```bash
make shaders_windows
```
in cmd line. You will need to install GNU coreutils and add it to your PATH variable.

# Additional Dependencies
https://github.com/nlohmann/json
Download the .hpp file from release and place it at `json/json.hpp`

# Graphics Debugging Tips (Mostly philosophical)
When doing graphics programming, sometimes it's better to first try things until it looks correct, and _then_ figure out why it looks correct. It takes far less time to test a hunch than it does to statically analyze why something looks wrong, and once you've got it looking right, you at least know that the logic you have must be explainable by some formula. This might be considered "programming by combinatorics" by some, but in the right situations it really can be faster.

When learning from other projects, first copy/paste the other project wholesale and make sure it actually runs, before trying to innovate on it. Graphics APIs are numerous and it's common for solutions to break when moved to a different platform. It could really be that it's not your code that's the issue, but the platform. Don't waste time chasing down a bug that might not have even been caused by a difference between your code and the original author's.

# Converting WSL 1 container to WSL 2
`wsl --set-version <containername> <version>`

# Visual Studio Code Tips
It complains if you're not in the ~/home directory. So create symlink and start `code .` there instead:

`ln -s /mnt/c/dev/learn_bgfx ~/home/learn_bgfx`

You might have to create the ~/home folder.

