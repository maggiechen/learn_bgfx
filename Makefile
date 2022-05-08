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
COMPILER_FLAGS = -w

# Source files
SOURCES = main.cpp getting_started/tutorial.cpp

# Where all the compiled, unlinked object files go
BIN = ./bin

# The final executable file
EXECUTABLE = $(BIN)/main

# Using := instead of = means that the variables in the value will be expanded at declaration time
# If you use =, it will only expand when it is used
OBJECTS := $(addprefix $(BIN)/, $(SOURCES:.cpp=.o))
# EXECUTABLE = $(BIN)/exec

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lGL -lX11 -ldl -lpthread -lrt

# Task: To create all the .o files in the bin folder, require that we have access to all the .cpp files.
# Execute the dir_guard task as an order-only prereq (i.e. After the pipe |, which means it create the folder if necessary,
# but not attempt to do this anytime a timestamp in the folder changes due to files being added/removed/renamed etc)
# The -c means only compile and do not link, and -o means override existing object files from previous builds.
# The $< means the first requirement for the task, i.e. the .cpp file being compiled
# patsubst (pattern substitute) means substitute the cpp with o in the cpp file's path. The % means wildcard.
$(BIN)/%.o: %.cpp | dir_guard
	$(CC) $(COMPILER_FLAGS) -c $< -o $(BIN)/$(patsubst %.cpp,%.o,$<)

# Since dir_guard is not an actual file that will be made by running it, it will be run
# on each make call. Assigning to .PHONY (reserved target) makes it so that it does not run each time.
.PHONY: dir_guard
dir_guard:
	@mkdir -p $(BIN)
	@mkdir -p $(BIN)/getting_started

# Link .o files into executable located at bin/main. Overwrite existing executable if necessary. Requires that objects have been compiled already
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(COMPILER_FLAGS) $(OBJECTS) -o $(BIN_DIR)/$@

# Requires that executable task has completed, logs a message in console saying it's done.
all : $(EXECUTABLE)
	@echo Make has finished.