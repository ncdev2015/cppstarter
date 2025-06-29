#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>

#include "utils/colors.hpp"

namespace fs = std::filesystem;

const std::string VERSION = "v2.2.0";

void show_help(const std::string& program_name) {    
    std::cout << "Usage:\n"
              << "  " << program_name << " new <ProjectName> [--init-git]    Create a new C++ project\n"
              << "  " << program_name << " run                               Run debug build\n"
              << "  " << program_name << " run-release                       Run release build\n"
              << "  " << program_name << " min                               Creates a minimal prompt script (min.sh)\n"
              << "  " << program_name << " --help                            Show this help message\n"
              << "  " << program_name << " --version                         Show version\n";
}

void show_version() {
    std::cout << "cppstarter version " << VERSION << '\n';
}

void run_build(const std::string& type) {
    std::string command;

    if (type == "debug") {
        command = "make run";
    } else if (type == "run-release") {
        command = "make run-release";
    } else {
        std::cerr << "Unknown build type: " << type << '\n';
        return;
    }

    std::cout << "Running " << type << " build via Makefile...\n";
    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Execution failed with code: " << result << '\n';
    }
}

void create_file(const fs::path& path, const std::string& content) {
    std::ofstream file(path);
    if (!file) {
        std::cerr << "Error: could not create file " << path << "\n";
        return;
    }
    file << content;
}

bool create_directory(const fs::path& path) {
    std::error_code ec;
    if (!fs::create_directories(path, ec) && ec) {
        std::cerr << "Warning: could not create directory " << path << ": " << ec.message() << "\n";
        return false;
    }
    return true;
}

void create_project(const std::string& project, bool init_git) {
    create_directory(project + "/src");
    create_directory(project + "/include");
    create_directory(project + "/tests");
    create_directory(project + "/build");

    create_file(project + "/src/main.cpp",
        "#include <iostream>\n\nint main(int argc, char* argv[]) {\n    std::cout << \"Hello, " + project + "!\" << std::endl;\n    return 0;\n}"
    );

    create_file(project + "/Makefile",
        "CXX = g++\n"
        "SRC = $(wildcard src/*.cpp)\n"
        "INCLUDES = -Iinclude\n\n"

        "# === Debug configuration ===\n"
        "DBG_FLAGS = -Wall $(INCLUDES) -g\n"
        "DBG_OBJ = $(patsubst src/%.cpp, build/debug/obj/%.o, $(SRC))\n"
        "DBG_BIN = build/debug/bin/" + project + "\n\n"

        "# === Release configuration ===\n"
        "REL_FLAGS = -Wall $(INCLUDES) -O2\n"
        "REL_OBJ = $(patsubst src/%.cpp, build/release/obj/%.o, $(SRC))\n"
        "REL_BIN = build/release/bin/" + project + "\n\n"

        "LIBS_DEBUG = \n"
        "LIBS_RELEASE = \n\n"

        "all: $(DBG_BIN)\n\n"

        "$(DBG_BIN): $(DBG_OBJ)\n"
        "\tmkdir -p $(dir $@)\n"
        "\t$(CXX) $(DBG_FLAGS) -o $@ $^ $(LIBS_DEBUG)\n\n"

        "build/debug/obj/%.o: src/%.cpp\n"
        "\tmkdir -p $(dir $@)\n"
        "\t$(CXX) $(DBG_FLAGS) -c $< -o $@\n\n"

        "release: $(REL_BIN)\n\n"

        "$(REL_BIN): $(REL_OBJ)\n"
        "\tmkdir -p $(dir $@)\n"
        "\t$(CXX) $(REL_FLAGS) -o $@ $^ $(LIBS_RELEASE)\n\n"

        "build/release/obj/%.o: src/%.cpp\n"
        "\tmkdir -p $(dir $@)\n"
        "\t$(CXX) $(REL_FLAGS) -c $< -o $@\n\n"

        "test:\n"
        "\tmkdir -p build/debug/bin\n"
        "\t$(CXX) $(DBG_FLAGS) -Itests -o build/debug/bin/test_math tests/test_math.cpp\n"
        "\t./build/debug/bin/test_math\n\n"

        "valgrind: $(DBG_BIN)\n"
        "\tvalgrind --leak-check=full --track-origins=yes ./$(DBG_BIN)\n\n"

        "clean:\n"
        "\trm -rf build\n\n"

        "run: $(DBG_BIN)\n"
        "\t./$(DBG_BIN)\n\n"

        "run-release: $(REL_BIN)\n"
        "\t./$(REL_BIN)"
    );

    create_file(project + "/tests/test_math.cpp",
        "#include <cassert>\n#include <iostream>\n\nint main() {\n    assert(2 + 2 == 4);\n    std::cout << \"All tests passed!\\n\";\n    return 0;\n}"
    );

    create_file(project + "/.gitignore", R"(/build/
/bin/
*.o
*.out
*.exe
*.log
*~
*.swp
.DS_Store
core
)");

    create_file(project + "/README.md", "# " + project + R"(

This is an automatically generated C++ project.

## Build

```bash
make
```

## Run

```bash
make run
```

## Run tests

```bash
make test
```

## Build release

```bash
make release
```

## Run release

```bash
make run-release
```

## Clean build files

```bash
make clean
```
)");

    if (init_git) {
        std::string cmd = "cd " + project + " && git init";
        int ret = std::system(cmd.c_str());
        if (ret != 0) {
            std::cerr << "Warning: failed to initialize git repository.\n";
        } else {
            std::cout << "Git repository initialized.\n";
        }
    }

    std::cout << "Project '" << project << "' created successfully.\n";
}

void create_min_sh() {
    std::ofstream script("min.sh");
    if (script.is_open()) {
        script << "#!/bin/bash\n";
        script << "export PS1='\\[\\e[1;34m\\]\\W\\$\\[\\e[0m\\] '\n";
        script.close();

        std::cout << "Script 'min.sh' created. To activate the reduced prompt, run:\n";
        std::cout << "    source ./min.sh\n";
    } else {
        std::cerr << "Could not create 'min.sh'\n";
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        show_help(argv[0]);
        return 1;
    }

    std::string cmd = argv[1];

    if (cmd == "--help") {
        show_help(argv[0]);
    } else if (cmd == "--version") {
        show_version();
    } else if (cmd == "run") {
        run_build("debug");
    } else if (cmd == "run-release") {
        run_build("run-release");
    } else if (cmd == "min") {
        create_min_sh();        
    } else if (cmd == "new") {
        if (argc < 3) {
            std::cerr << "Error: Missing project name.\n";
            return 1;
        }
        std::string project_name = argv[2];
        bool init_git = (argc == 4 && std::string(argv[3]) == "--init-git");
        create_project(project_name, init_git);
    } else {
        std::cerr << "Error: Unknown command '" << cmd << "'.\n";
        show_help(argv[0]);
        return 1;
    }

    return 0;
}
