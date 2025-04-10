#DeepSeek tohle umí líp něž ChatGPT :)

SRC_DIR = src
BUILD_DIR = build
CC = g++
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)  # Find all .cpp files in the src directory
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))  # Generate corresponding .o files
OBJ_NAME = RayTracing

# Include paths for SDL2 and SDL2_image headers
INCLUDE_PATHS = -I/opt/homebrew/include/SDL2 -I/opt/homebrew/Cellar/sdl2_image/2.8.8/include/SDL2

# Library paths for SDL2 and SDL2_image
LIBRARY_PATHS = -L/opt/homebrew/lib -L/opt/homebrew/Cellar/sdl2_image/2.8.8/lib

# Compiler flags
COMPILER_FLAGS = -std=c++17 -Wall -Wextra -O3

# Linker flags for SDL2 and SDL2_image
LINKER_FLAGS = -lSDL2 -lSDL2_image

# Default target
all: clean $(BUILD_DIR)/$(OBJ_NAME)

# Rule to build the final executable
$(BUILD_DIR)/$(OBJ_NAME): $(OBJ_FILES)
	$(CC) $(COMPILER_FLAGS) $(LIBRARY_PATHS) $^ -o $@ $(LINKER_FLAGS)

# Rule to compile each .cpp file into an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) -c $< -o $@

# Rule to create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean rule to remove the build directory and its contents
clean:
	rm -rf $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)

.PHONY: all clean