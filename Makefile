# CC specifies which compiler we're using
CC = g++

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lGL -lX11 -ldl -lpthread -lrt
BIN="./bin"

# This is the target that compiles our executable
all : main.cpp
	$(CC) main.cpp -o $(BIN)/main $(COMPILER_FLAGS) $(LINKER_FLAGS)
