CXX = g++
OPTIMIZATION_LEVEL = -O2
SRC = $(wildcard src/*.cpp)
INCLUDES = -Iinclude

# === Google Test configuration ===
GTEST_DIR = build/googletest
GTEST_REPO = https://github.com/google/googletest.git
GTEST_TAG = v1.14.0
GTEST_INCLUDE = -I$(GTEST_DIR)/googletest/include
GTEST_LIB = $(GTEST_DIR)/build/lib/libgtest.a $(GTEST_DIR)/build/lib/libgtest_main.a

# Test source files
TEST_SRC = $(wildcard test/*.cpp tests/*.cpp)
TEST_MAIN_SRC = $(filter-out %main.cpp, $(SRC))  # Exclude main.cpp for tests

# === Debug configuration ===
DBG_FLAGS = -Wall $(INCLUDES) -g -std=c++17
DBG_OBJ = $(patsubst src/%.cpp, build/debug/obj/%.o, $(SRC))
DBG_BIN = build/debug/bin/cppstarter

# === Release configuration ===
REL_FLAGS = -Wall $(INCLUDES) $(OPTIMIZATION_LEVEL) -std=c++17
REL_OBJ = $(patsubst src/%.cpp, build/release/obj/%.o, $(SRC))
REL_BIN = build/release/bin/cppstarter

# === Test configuration ===
TEST_FLAGS = -Wall $(INCLUDES) $(GTEST_INCLUDE) -g -std=c++17 -pthread
TEST_OBJ = $(patsubst src/%.cpp, build/test/obj/%.o, $(TEST_MAIN_SRC))
TEST_BIN = build/test/bin/test_runner

LIBS_DEBUG = 
LIBS_RELEASE = 
LIBS_TEST = $(GTEST_LIB) -pthread

all: $(DBG_BIN)

# === Debug build ===
$(DBG_BIN): $(DBG_OBJ)
	mkdir -p $(dir $@)
	$(CXX) $(DBG_FLAGS) -o $@ $^ $(LIBS_DEBUG)

build/debug/obj/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(DBG_FLAGS) -c $< -o $@

# === Release build ===
release: $(REL_BIN)

$(REL_BIN): $(REL_OBJ)
	mkdir -p $(dir $@)
	$(CXX) $(REL_FLAGS) -o $@ $^ $(LIBS_RELEASE)

build/release/obj/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(REL_FLAGS) -c $< -o $@

# === Google Test setup ===
$(GTEST_DIR):
	@echo "Downloading Google Test..."
	git clone --depth 1 --branch $(GTEST_TAG) $(GTEST_REPO) $(GTEST_DIR)

$(GTEST_LIB): $(GTEST_DIR)
	@echo "Building Google Test..."
	mkdir -p $(GTEST_DIR)/build
	cd $(GTEST_DIR)/build && \
	cmake -DCMAKE_BUILD_TYPE=Release \
	      -DBUILD_GMOCK=OFF \
	      -DINSTALL_GTEST=OFF \
	      -DGTEST_HAS_PTHREAD=1 \
	      .. && \
	$(MAKE)

# === Test build ===
$(TEST_BIN): $(GTEST_LIB) $(TEST_OBJ)
	@if [ -z "$(TEST_SRC)" ]; then \
		echo "No test files found in test/ or tests/ directories"; \
		echo "Please create test files (*.cpp) in test/ or tests/"; \
		exit 1; \
	fi
	mkdir -p $(dir $@)
	$(CXX) $(TEST_FLAGS) -o $@ $(TEST_OBJ) $(TEST_SRC) $(LIBS_TEST)

build/test/obj/%.o: src/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(TEST_FLAGS) -c $< -o $@

# === Test targets ===
test: $(TEST_BIN)
	@echo "Running tests..."
	./$(TEST_BIN)

test-verbose: $(TEST_BIN)
	@echo "Running tests with verbose output..."
	./$(TEST_BIN) --gtest_output=xml:build/test/results.xml

test-filter: $(TEST_BIN)
	@echo "Running filtered tests (use FILTER=pattern)..."
	./$(TEST_BIN) --gtest_filter=$(FILTER)

test-list: $(TEST_BIN)
	@echo "Available tests:"
	./$(TEST_BIN) --gtest_list_tests

# === Valgrind targets ===
valgrind: $(DBG_BIN)
	valgrind --leak-check=full --track-origins=yes --error-exitcode=1 ./$(DBG_BIN)

valgrind-detailed: $(DBG_BIN)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	         --track-fds=yes --show-reachable=yes --error-exitcode=1 ./$(DBG_BIN)

test-valgrind: $(TEST_BIN)
	@echo "Running tests with Valgrind..."
	valgrind --leak-check=full --track-origins=yes --error-exitcode=1 ./$(TEST_BIN)

test-valgrind-detailed: $(TEST_BIN)
	@echo "Running tests with detailed Valgrind analysis..."
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	         --track-fds=yes --show-reachable=yes --error-exitcode=1 ./$(TEST_BIN)

# === Run targets ===
# Colored output for the binary output
CYAN := \033[36m
GREEN := \033[32m
YELLOW := \033[33m
RED := \033[31m
RESET := \033[0m

run: $(DBG_BIN)
	@echo -e "$(GREEN)Running debug build...$(RESET)"
	@./$(DBG_BIN) | awk -v cyan="$(CYAN)" -v reset="$(RESET)" '{print cyan $$0 reset}'

run-release: $(REL_BIN)
	@echo -e "$(GREEN)Running release build...$(RESET)"
	@./$(REL_BIN)

# === Setup targets ===
setup-gtest: $(GTEST_LIB)
	@echo -e "$(GREEN)Google Test setup complete!$(RESET)"

# === Clean targets ===
clean:
	rm -rf build

clean-gtest:
	rm -rf $(GTEST_DIR)

clean-all: clean clean-gtest

# === Installation ===
PREFIX ?= /usr/local

install: $(REL_BIN)
	mkdir -p $(PREFIX)/bin
	cp $(REL_BIN) $(PREFIX)/bin/cppstarter

uninstall:
	rm -f $(PREFIX)/bin/cppstarter

# === Help ===
help: ## Shows this help
	@echo -e "$(YELLOW)Available targets:$(RESET)"
	@echo "  $(GREEN)Build targets:$(RESET)"
	@echo "    all         - Build debug application (default)"
	@echo "    release     - Build optimized release application"
	@echo ""
	@echo "  $(GREEN)Test targets:$(RESET)"
	@echo "    setup-gtest - Download and build Google Test"
	@echo "    test        - Build and run all tests"
	@echo "    test-verbose- Run tests with XML output"
	@echo "    test-filter - Run filtered tests (use FILTER=pattern)"
	@echo "    test-list   - List all available tests"
	@echo ""
	@echo "  $(GREEN)Memory analysis:$(RESET)"
	@echo "    valgrind    - Run debug app with Valgrind"
	@echo "    valgrind-detailed - Run with detailed Valgrind analysis"
	@echo "    test-valgrind - Run tests with Valgrind"
	@echo "    test-valgrind-detailed - Run tests with detailed Valgrind"
	@echo ""
	@echo "  $(GREEN)Run targets:$(RESET)"
	@echo "    run         - Run debug application (with colored output)"
	@echo "    run-release - Run release application"
	@echo ""
	@echo "  $(GREEN)Maintenance:$(RESET)"
	@echo "    clean       - Remove build files"
	@echo "    clean-gtest - Remove Google Test files"
	@echo "    clean-all   - Remove all build and test files"
	@echo "    install     - Install release binary to system"
	@echo "    uninstall   - Remove installed binary"
	@echo "    help        - Show this help"
	@echo ""
	@echo "  $(GREEN)Options:$(RESET)"
	@echo "    PREFIX      - Installation prefix (default: /usr/local)"
	@echo "    FILTER      - Test filter pattern for test-filter target"
	@echo ""
	@echo "  $(GREEN)Examples:$(RESET)"
	@echo "    make test FILTER='*Math*'  - Run only Math tests"
	@echo "    make install PREFIX=/opt   - Install to /opt/bin"

.PHONY: all release test test-verbose test-filter test-list setup-gtest \
        run run-release valgrind valgrind-detailed test-valgrind test-valgrind-detailed \
        install uninstall clean clean-gtest clean-all help