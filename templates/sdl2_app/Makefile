CXX = g++
OPTIMIZATION_LEVEL = -O2
SRC = $(wildcard src/*.cpp)

# Automatically get flags and libraries using sdl2-config
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LIBS   = $(shell sdl2-config --libs)
INCLUDES = -Iinclude

# === Debug configuration ===
DBG_FLAGS = -Wall $(INCLUDES) $(SDL_CFLAGS) -g
DBG_OBJ = $(patsubst src/%.cpp, build/debug/obj/%.o, $(SRC))
DBG_BIN = build/debug/bin/sdl2_app
LIBS_DEBUG = $(SDL_LIBS)

# === Release configuration ===
REL_FLAGS = -Wall $(INCLUDES) $(SDL_CFLAGS) $(OPTIMIZATION_LEVEL)
REL_OBJ = $(patsubst src/%.cpp, build/release/obj/%.o, $(SRC))
REL_BIN = build/release/bin/sdl2_app
LIBS_RELEASE = $(SDL_LIBS)

all: $(DBG_BIN)

$(DBG_BIN): $(DBG_OBJ)
	mkdir -p $(dir $@)
	$(CXX) $(DBG_FLAGS) -o $@ $^ $(LIBS_DEBUG)

build/debug/obj/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(DBG_FLAGS) -c $< -o $@

release: $(REL_BIN)

$(REL_BIN): $(REL_OBJ)
	mkdir -p $(dir $@)
	$(CXX) $(REL_FLAGS) -o $@ $^ $(LIBS_RELEASE)

build/release/obj/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(REL_FLAGS) -c $< -o $@

test:
	mkdir -p build/debug/bin
	$(CXX) $(DBG_FLAGS) -Itests -o build/debug/bin/test_math tests/test_math.cpp
	./build/debug/bin/test_math

valgrind: $(DBG_BIN)
	valgrind --leak-check=full --track-origins=yes ./$(DBG_BIN)

run: $(DBG_BIN)
	./$(DBG_BIN)

run-release: $(REL_BIN)
	./$(REL_BIN)

clean:
	rm -rf build

help: ## Shows this help
	@echo "Available targets:"
	@echo "  all         - Build debug version (default)"
	@echo "  release     - Build optimized SDL2 application"
	@echo "  test        - Compile and run tests"
	@echo "  run         - Run SDL2 application in debug mode"
	@echo "  run-release - Run SDL2 application in release mode"
	@echo "  valgrind    - Run debug application with valgrind"
	@echo "  clean       - Remove all compiled files and directories"
	@echo "  help        - Show this help"

.PHONY: all release test run run-release valgrind clean help