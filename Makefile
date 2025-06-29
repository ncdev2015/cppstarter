CXX = g++
OPTIMIZATION_LEVEL = -O2

SRC = $(wildcard src/*.cpp)
INCLUDES = -Iinclude

# === Debug configuration ===
DBG_FLAGS = -Wall $(INCLUDES) -g
DBG_OBJ = $(patsubst src/%.cpp, build/debug/obj/%.o, $(SRC))
DBG_BIN = build/debug/bin/cppstarter

# === Release configuration ===
REL_FLAGS = -Wall $(INCLUDES) $(OPTIMIZATION_LEVEL)
REL_OBJ = $(patsubst src/%.cpp, build/release/obj/%.o, $(SRC))
REL_BIN = build/release/bin/cppstarter

LIBS_DEBUG = 
LIBS_RELEASE = 

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

# Colored output for the binary output
CYAN := \033[36m
RESET := \033[0m

run: $(DBG_BIN)
	@./$(DBG_BIN) | awk -v cyan="$(CYAN)" -v reset="$(RESET)" '{print cyan $$0 reset}'

run-release: $(REL_BIN)
	./$(REL_BIN)

clean:
	rm -rf build

PREFIX ?= /usr/local

# Install the release binary to the system's bin directory (default: /usr/local/bin)
# Usage: sudo make install
install: $(REL_BIN)
	mkdir -p $(PREFIX)/bin
	cp $(REL_BIN) $(PREFIX)/bin/cppstarter

