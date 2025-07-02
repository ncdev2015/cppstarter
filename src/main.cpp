#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>
#include <string_view>
#include <array>
#include <unordered_map>
#include <functional>

#include "utils/colors.hpp"

namespace fs = std::filesystem;

constexpr std::string_view PROGRAM_NAME = "cppstarter";
constexpr std::string_view VERSION = "v2.3.0";

// Forward declarations
void show_help(std::string_view program_name);
void show_version(std::string_view program_name);
void create_project_handler(std::string_view program_name);
void run_build();
void run_debug();
void run_release();
void run_tests();
void run_valgrind();
void create_min_sh();

// Command handler type
using CommandHandler = std::function<void()>;

// Command registry using unordered_map for O(1) lookup
const std::unordered_map<std::string_view, CommandHandler> commands = {
    {"--help", []() { show_help(PROGRAM_NAME); }},
    {"--version", []() { show_version(PROGRAM_NAME); }},
    {"build", run_build},
    {"run", run_debug},
    {"run-release", run_release},
    {"test", run_tests},
    {"valgrind", run_valgrind},
    {"min", create_min_sh}
};

void show_help(std::string_view program_name) {
    std::cout << colors::GREEN
              << "Usage:\n"
              << "  " << program_name << " new <ProjectName> [--init-git]    Create a new C++ project\n"
              << "  " << program_name << " build                             Compile debug build\n"
              << "  " << program_name << " run                               Run debug build\n"
              << "  " << program_name << " run-release                       Run release build\n"
              << "  " << program_name << " test                              Compile and run tests\n"
              << "  " << program_name << " valgrind                          Run debug application with valgrind\n"
              << "  " << program_name << " min                               Creates a minimal prompt script (min.sh)\n"
              << "  " << program_name << " --help                            Show this help message\n"
              << "  " << program_name << " --version                         Show version\n"
              << colors::RESET;
}

void show_version(std::string_view program_name) {
    std::cout << PROGRAM_NAME << " version " << VERSION << '\n';
}

bool execute_system_command(const std::string& command, const std::string& description = "") {
    if (!description.empty()) {
        std::cout << colors::CYAN << description << colors::RESET << '\n';
    }
    
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cout << colors::RED
                  << "Error: Command '" << command << "' failed with code: " << result
                  << colors::RESET << '\n';
        return false;
    }
    return true;
}

void run_build() {
    execute_system_command("make", "Compiling debug build...");
}

void run_debug() {
    execute_system_command("make run", "Running debug build...");
}

void run_release() {
    execute_system_command("make run-release", "Running release build...");
}

void run_tests() {
    execute_system_command("make test", "Running tests...");
}

void run_valgrind() {
    execute_system_command("make valgrind", "Running with valgrind...");
}

void create_file(const fs::path& path, std::string_view content) {
    try {
        std::ofstream file(path);
        if (!file) {
            throw std::runtime_error("Could not create file: " + path.string());
        }
        file << content;
        file.close();
    } catch (const std::exception& e) {
        std::cout << colors::RED
                  << "Error creating file " << path << ": " << e.what()
                  << colors::RESET << '\n';
    }
}

bool create_directory(const fs::path& path) {
    std::error_code ec;
    if (!fs::create_directories(path, ec) && ec) {
        std::cout << colors::YELLOW
                  << "Warning: Could not create directory " << path << ": " << ec.message()
                  << colors::RESET << '\n';
        return false;
    }
    return true;
}

void create_project(const std::string& project_name, bool init_git = false) {
    if (project_name.empty()) {
        std::cout << colors::RED << "Error: Project name cannot be empty" << colors::RESET << '\n';
        return;
    }

    if (fs::exists(project_name)) {
        std::cout << colors::RED 
                  << "Error: Directory '" << project_name << "' already exists" 
                  << colors::RESET << '\n';
        return;
    }

    std::cout << colors::CYAN << "Creating project '" << project_name << "'..." << colors::RESET << '\n';

    // Create directory structure
    const std::vector<std::string> directories = {
        project_name + "/src",
        project_name + "/include", 
        project_name + "/tests",
        project_name + "/build"
    };

    for (const auto& dir : directories) {
        if (!create_directory(dir)) {
            return;
        }
    }

    // Create main.cpp
    create_file(project_name + "/src/main.cpp",
        "#include <iostream>\n\n"
        "int main(int argc, char* argv[]) {\n"
        "    // Suppress unused parameter warnings\n"
        "    (void)argc;\n"
        "    (void)argv;\n"
        "    \n"
        "    std::cout << \"Hello, " + project_name + "!\" << std::endl;\n"
        "    return 0;\n"
        "}\n"
    );

    // Create Makefile
    const std::string makefile_content = 
        "CXX = g++\n"
        "CXXFLAGS = -std=c++17\n"
        "SRC = $(wildcard src/*.cpp)\n"
        "INCLUDES = -Iinclude\n\n"

        "# === Debug configuration ===\n"
        "DBG_FLAGS = -Wall -Wextra -Wpedantic $(INCLUDES) -g -DDEBUG\n"
        "DBG_OBJ = $(patsubst src/%.cpp, build/debug/obj/%.o, $(SRC))\n"
        "DBG_BIN = build/debug/bin/" + project_name + "\n\n"

        "# === Release configuration ===\n"
        "REL_FLAGS = -Wall -Wextra $(INCLUDES) -O2 -DNDEBUG\n"
        "REL_OBJ = $(patsubst src/%.cpp, build/release/obj/%.o, $(SRC))\n"
        "REL_BIN = build/release/bin/" + project_name + "\n\n"

        "# Libraries\n"
        "LIBS_DEBUG = \n"
        "LIBS_RELEASE = \n\n"

        "# Default target\n"
        ".PHONY: all clean run run-release test valgrind\n\n"

        "all: $(DBG_BIN)\n\n"

        "# Debug build\n"
        "$(DBG_BIN): $(DBG_OBJ)\n"
        "\t@mkdir -p $(dir $@)\n"
        "\t$(CXX) $(CXXFLAGS) $(DBG_FLAGS) -o $@ $^ $(LIBS_DEBUG)\n"
        "\t@echo \"Debug build complete: $@\"\n\n"

        "build/debug/obj/%.o: src/%.cpp\n"
        "\t@mkdir -p $(dir $@)\n"
        "\t$(CXX) $(CXXFLAGS) $(DBG_FLAGS) -c $< -o $@\n\n"

        "# Release build\n"
        "release: $(REL_BIN)\n\n"

        "$(REL_BIN): $(REL_OBJ)\n"
        "\t@mkdir -p $(dir $@)\n"
        "\t$(CXX) $(CXXFLAGS) $(REL_FLAGS) -o $@ $^ $(LIBS_RELEASE)\n"
        "\t@echo \"Release build complete: $@\"\n\n"

        "build/release/obj/%.o: src/%.cpp\n"
        "\t@mkdir -p $(dir $@)\n"
        "\t$(CXX) $(CXXFLAGS) $(REL_FLAGS) -c $< -o $@\n\n"

        "# Test target\n"
        "test: build/debug/bin/test_runner\n"
        "\t@echo \"Running tests...\"\n"
        "\t@./build/debug/bin/test_runner\n\n"

        "build/debug/bin/test_runner: tests/test_math.cpp\n"
        "\t@mkdir -p $(dir $@)\n"
        "\t$(CXX) $(CXXFLAGS) $(DBG_FLAGS) -Itests -o $@ $<\n\n"

        "# Valgrind target\n"
        "valgrind: $(DBG_BIN)\n"
        "\t@echo \"Running with valgrind...\"\n"
        "\tvalgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(DBG_BIN)\n\n"

        "# Run targets\n"
        "run: $(DBG_BIN)\n"
        "\t@echo \"Running debug build...\"\n"
        "\t@./$(DBG_BIN)\n\n"

        "run-release: $(REL_BIN)\n"
        "\t@echo \"Running release build...\"\n"
        "\t@./$(REL_BIN)\n\n"

        "# Clean target\n"
        "clean:\n"
        "\t@echo \"Cleaning build files...\"\n"
        "\t@rm -rf build\n\n"

        "# Help target\n"
        "help:\n"
        "\t@echo \"Available targets:\"\n"
        "\t@echo \"  all        - Build debug version (default)\"\n"
        "\t@echo \"  release    - Build release version\"\n"
        "\t@echo \"  run        - Build and run debug version\"\n"
        "\t@echo \"  run-release- Build and run release version\"\n"
        "\t@echo \"  test       - Build and run tests\"\n"
        "\t@echo \"  valgrind   - Run debug build with valgrind\"\n"
        "\t@echo \"  clean      - Remove build files\"\n"
        "\t@echo \"  help       - Show this help\"\n";

    create_file(project_name + "/Makefile", makefile_content);

    // Create test file
    create_file(project_name + "/tests/test_math.cpp",
        "#include <cassert>\n"
        "#include <iostream>\n\n"
        "// Simple test framework\n"
        "void test_basic_math() {\n"
        "    assert(2 + 2 == 4);\n"
        "    assert(5 * 3 == 15);\n"
        "    assert(10 - 7 == 3);\n"
        "    std::cout << \"✓ Basic math tests passed\" << std::endl;\n"
        "}\n\n"
        "int main() {\n"
        "    std::cout << \"Running tests...\" << std::endl;\n"
        "    \n"
        "    try {\n"
        "        test_basic_math();\n"
        "        std::cout << \"\\n✅ All tests passed!\" << std::endl;\n"
        "        return 0;\n"
        "    } catch (const std::exception& e) {\n"
        "        std::cout << \"❌ Test failed: \" << e.what() << std::endl;\n"
        "        return 1;\n"
        "    }\n"
        "}\n"
    );

    // Create README
    const std::string readme_content = "# " + project_name + "\n\n"
        "This is an automatically generated C++ project using modern C++17 standards.\n\n"
        "## Quick Start\n\n"
        "```bash\n"
        "# Build and run debug version\n"
        "make run\n\n"
        "# Build and run release version\n"
        "make run-release\n\n"
        "# Run tests\n"
        "make test\n\n"
        "# Memory analysis with valgrind\n"
        "make valgrind\n"
        "```\n\n"
        "## Build System\n\n"
        "This project uses a Makefile with the following targets:\n\n"
        "- `make` or `make all` - Build debug version\n"
        "- `make release` - Build optimized release version\n"
        "- `make run` - Build and run debug version\n"
        "- `make run-release` - Build and run release version\n"
        "- `make test` - Build and run tests\n"
        "- `make valgrind` - Run debug build with memory analysis\n"
        "- `make clean` - Remove all build files\n"
        "- `make help` - Show available targets\n\n"
        "## Project Structure\n\n"
        "```\n" + project_name + "/\n"
        "├── src/           # Source files\n"
        "├── include/       # Header files\n"
        "├── tests/         # Test files\n"
        "├── build/         # Build artifacts (auto-generated)\n"
        "├── Makefile       # Build configuration\n"
        "└── README.md      # This file\n"
        "```\n\n"
        "## Compiler Flags\n\n"
        "- **Debug**: `-Wall -Wextra -Wpedantic -g -DDEBUG`\n"
        "- **Release**: `-Wall -Wextra -O2 -DNDEBUG`\n"
        "- **Standard**: C++17\n\n"
        "## Dependencies\n\n"
        "- GCC/Clang with C++17 support\n"
        "- Make\n"
        "- Valgrind (optional, for memory analysis)\n";

    create_file(project_name + "/README.md", readme_content);

    // Create .gitignore
    create_file(project_name + "/.gitignore", 
        "# Build artifacts\n"
        "/build/\n"
        "/bin/\n"
        "*.o\n"
        "*.out\n"
        "*.exe\n\n"

        "# IDE files\n"
        ".vscode/\n"
        ".idea/\n"
        "*.swp\n"
        "*.swo\n"
        "*~\n\n"

        "# System files\n"
        ".DS_Store\n"
        "Thumbs.db\n\n"

        "# Debug files\n"
        "*.log\n"
        "core\n"
        "vgcore.*\n\n"

        "# Temporary files\n"
        "*.tmp\n"
        "*.temp\n"
    );

    // Initialize git if requested
    if (init_git) {
        std::string git_cmd = "cd " + project_name + " && git init && git add . && git commit -m \"Initial commit\"";
        if (execute_system_command(git_cmd, "Initializing git repository...")) {
            std::cout << colors::GREEN << "Git repository initialized with initial commit" << colors::RESET << '\n';
        }
    }

    std::cout << colors::GREEN 
              << "✅ Project '" << project_name << "' created successfully!" << colors::RESET << '\n';
    std::cout << colors::CYAN 
              << "Next steps:\n"
              << "  cd " << project_name << "\n"
              << "  make run" << colors::RESET << '\n';
}

void create_min_sh() {
    try {
        std::ofstream script("min.sh");
        if (!script) {
            throw std::runtime_error("Could not create min.sh");
        }
        
        script << "#!/bin/bash\n"
               << "# Minimal bash prompt\n"
               << "export PS1='\\[\\e[1;34m\\]\\W\\$\\[\\e[0m\\] '\n"
               << "echo \"Minimal prompt activated. Type 'exit' to return to normal prompt.\"\n";
        
        script.close();
        
        // Make script executable
        execute_system_command("chmod +x min.sh");
        
        std::cout << colors::GREEN
                  << "✅ Script 'min.sh' created successfully!\n"
                  << "To activate the minimal prompt, run:\n"
                  << "    source ./min.sh"
                  << colors::RESET << '\n';
                  
    } catch (const std::exception& e) {
        std::cout << colors::RED 
                  << "Error: " << e.what() 
                  << colors::RESET << '\n';
    }
}

void create_project_handler(const std::string& project_name) {
    create_project(project_name);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        show_help(argv[0]);
        return 0;
    }

    std::string_view command = argv[1];

    // Handle 'new' command separately since it requires arguments
    if (command == "new") {
        if (argc < 3) {
            std::cout << colors::RED 
                      << "Error: 'new' command requires a project name\n"
                      << "Usage: " << argv[0] << " new <ProjectName> [--init-git]"
                      << colors::RESET << '\n';
            return 1;
        }
        
        std::string project_name = argv[2];
        bool init_git = (argc > 3 && std::string(argv[3]) == "--init-git");
        
        create_project(project_name, init_git);
        return 0;
    }

    // Handle other commands
    auto it = commands.find(command);
    if (it != commands.end()) {
        try {
            it->second(); // Execute the command handler
        } catch (const std::exception& e) {
            std::cout << colors::RED 
                      << "Error executing command: " << e.what() 
                      << colors::RESET << '\n';
            return 1;
        }
    } else {
        std::cout << colors::RED 
                  << "Error: Unknown command '" << command << "'\n"
                  << "Use '" << argv[0] << " --help' for usage information"
                  << colors::RESET << '\n';
        return 1;
    }

    return 0;
}