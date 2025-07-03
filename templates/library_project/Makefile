CXX = g++
OPTIMIZATION_LEVEL = -O2
# Source files for the library: all except main.cpp
SRC = $(filter-out src/main.cpp, $(wildcard src/*.cpp))
MAIN = src/main.cpp
INCLUDES = -Iinclude

# === Debug configuration ===
DBG_FLAGS = -Wall $(INCLUDES) -g
DBG_OBJ = $(patsubst src/%.cpp, build/debug/obj/%.o, $(SRC))
DBG_LIB = build/debug/lib/liblibrary_project.a
DBG_BIN = build/debug/bin/library_project

# === Release configuration ===
REL_FLAGS = -Wall $(INCLUDES) $(OPTIMIZATION_LEVEL)
REL_OBJ = $(patsubst src/%.cpp, build/release/obj/%.o, $(SRC))
REL_LIB = build/release/lib/liblibrary_project.a
REL_BIN = build/release/bin/library_project

all: $(DBG_BIN)

# === Build static library Debug ===
$(DBG_LIB): $(DBG_OBJ)
	mkdir -p $(dir $@)
	ar rcs $@ $^

# === Build test executable Debug ===
$(DBG_BIN): $(DBG_LIB)
	mkdir -p $(dir $@)
	$(CXX) $(DBG_FLAGS) -o $@ $(MAIN) -L$(dir $<) -llibrary_project

build/debug/obj/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(DBG_FLAGS) -c $< -o $@

# === Build static library Release ===
release: $(REL_BIN)

$(REL_LIB): $(REL_OBJ)
	mkdir -p $(dir $@)
	ar rcs $@ $^

# === Build test executable Release ===
$(REL_BIN): $(REL_LIB)
	mkdir -p $(dir $@)
	$(CXX) $(REL_FLAGS) -o $@ $(MAIN) -L$(dir $<) -llibrary_project

build/release/obj/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(REL_FLAGS) -c $< -o $@

# === Run unit tests (example) ===
test:
	mkdir -p build/debug/bin
	$(CXX) $(DBG_FLAGS) -Itests -o build/debug/bin/test_math tests/test_math.cpp
	./build/debug/bin/test_math

# === Run with valgrind ===
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
	@echo "  release     - Build optimized release library and executable"
	@echo "  test        - Compile and run unit tests"
	@echo "  run         - Run main executable in debug mode"
	@echo "  run-release - Run main executable in release mode"
	@echo "  valgrind    - Run debug executable with valgrind"
	@echo "  clean       - Remove all compiled files and directories"
	@echo "  help        - Show this help"

.PHONY: all release test run run-release valgrind clean help