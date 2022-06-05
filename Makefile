# Everything in a makefile that has BLAHBLAH = whatever format is a macro
# Everything that has this format:
# target: req1 req2 ... | ordered_req1 ordered_req2 ...
# 	step1
#   step2
# 	...

# is a rule that runs and creates the target if it does not exist. If running
# does not create the target, this rule will be run every time you call make

# CC specifies which compiler we're using
CC = g++

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# define BX_CONFIG_DEBUG as it's a macro required to include bx/math.h
# define we're using linux. There's some conditional includes in the project. Specifically the Nuget package for SDL2 uses "SDL/" but the apt package we use on Ubuntu is "SDL2/"
COMPILER_FLAGS = -w -DBX_CONFIG_DEBUG -DLINUX -std=c++11

# Source files
SOURCES = main.cpp getting_started/LearnBgfx.cpp \
    getting_started/GeometryLoader.cpp \
    getting_started/PosColorVertex.cpp \
    getting_started/ShaderLoader.cpp \
    getting_started/SquarePrimitive.cpp \
    getting_started/InputManager.cpp \
    getting_started/CameraNavigation.cpp \
    getting_started/TimerTicker.cpp
    


# Where all the compiled, unlinked object files go
BIN = ./bin

# The final executable file
EXECUTABLE = $(BIN)/main

# Using := instead of = means that the variables in the value will be expanded at declaration time
# If you use =, it will only expand when it is used
OBJECTS := $(addprefix $(BIN)/, $(SOURCES:.cpp=.o))
# EXECUTABLE = $(BIN)/exec

# LINKER_FLAGS_* specifies the libraries we're linking against
# Note that we add SDL2 and also the shared release bgfx library. The .so extension means it's a shared library
BGFX_DEBUG = bgfx/.build/linux64_gcc/bin/libbgfx-shared-libDebug.so
BGFX_RELEASE = bgfx/.build/linux64_gcc/bin/libbgfx-shared-libRelease.so
LINKER_FLAGS_DEBUG = $(BGFX_DEBUG) -lSDL2 -lGL -lX11 -ldl -lpthread -lrt
LINKER_FLAGS_RELEASE = $(BGFX_RELEASE) -lSDL2 -lGL -lX11 -ldl -lpthread -lrt

# BGFX headers
BGFX_HEADERS = -Ibgfx/include -Ibx/include -Ibimg/include

JSON_LIBRARY_HEADER = -Ijson/

# Task: To create all the .o files in the bin folder, require that we have access to all the .cpp files.
# Execute the dir_guard task as an order-only prereq (i.e. After the pipe |, which means it create the folder if necessary,
# but not attempt to do this anytime a timestamp in the folder changes due to files being added/removed/renamed etc)
# The -c means only compile and do not link, and -o means override existing object files from previous builds.
# The $< means the first requirement for the task, i.e. the .cpp file being compiled
# patsubst (pattern substitute) means substitute the cpp with o in the cpp file's path. The % means wildcard.
$(BIN)/%.o: %.cpp | dir_guard
	$(CC) -c $< -o $(BIN)/$(patsubst %.cpp,%.o,$<) $(COMPILER_FLAGS) $(BGFX_HEADERS) $(JSON_LIBRARY_HEADER)

# Since dir_guard is not an actual file that will be made by running it, it will be run
# on each make call. Assigning to .PHONY (reserved target) makes it so that it does not run each time.
.PHONY: dir_guard
dir_guard:
	@mkdir -p $(BIN)
	@mkdir -p $(BIN)/getting_started

# Link .o files into executable located at bin/main. Overwrite existing executable if necessary. Requires that objects have been compiled already
$(EXECUTABLE): $(OBJECTS)
ifeq ($(ARGS),debug)
	$(CC) $(OBJECTS) -o $(BIN_DIR)/$@ $(COMPILER_FLAGS) $(LINKER_FLAGS_DEBUG) $(BGFX_HEADERS) $(JSON_LIBRARY_HEADER)
else
	$(CC) $(OBJECTS) -o $(BIN_DIR)/$@ $(COMPILER_FLAGS) $(LINKER_FLAGS_RELEASE) $(BGFX_HEADERS) $(JSON_LIBRARY_HEADER)
endif

# Requires that executable task has completed, logs a message in console saying it's done.
# Recompile the shaders into .bin files.
all : $(EXECUTABLE)
	./bgfx/.build/linux64_gcc/bin/shadercRelease \
    -f getting_started/v_simple.sc \
    -o v_simple.bin \
    --platform linux \
    --type vertex \
    --verbose \
    -i bgfx/src \
	--varyingdef getting_started/varying.def.sc
	./bgfx/.build/linux64_gcc/bin/shadercRelease \
    -f getting_started/f_simple.sc \
    -o f_simple.bin \
    --platform linux \
    --type fragment \
    --verbose \
    -i bgfx/src \
    --varyingdef getting_started/varying.def.sc
	@echo Make has finished.

.PHONY: shaders_windows
shaders_windows:
	./bgfx/.build/win64_vs2017/bin/shadercRelease \
    -f getting_started/v_simple.sc \
    -o v_simple.bin \
    --platform windows \
    --type vertex \
    --verbose \
    -i bgfx/src \
	--varyingdef getting_started/varying.def.sc
	./bgfx/.build/win64_vs2017/bin/shadercRelease \
    -f getting_started/f_simple.sc \
    -o f_simple.bin \
    --platform windows \
    --type fragment \
    --verbose \
    -i bgfx/src \
    --varyingdef getting_started/varying.def.sc
	@echo Shaders compiled
