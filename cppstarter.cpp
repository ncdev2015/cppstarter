#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

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

int main(int argc, char* argv[]) {
    if (argc < 3 || std::string(argv[1]) != "new") {
        std::cerr << "Usage: " << argv[0] << " new <ProjectName> [--init-git]\n";
        return 1;
    }

    std::string project = argv[2];
    bool init_git = (argc == 4 && std::string(argv[3]) == "--init-git");

    // Create directories
    create_directory(project + "/src");
    create_directory(project + "/include");
    create_directory(project + "/tests");
    create_directory(project + "/bin");
    create_directory(project + "/build");

    // main.cpp
    create_file(project + "/src/main.cpp",
        "#include <iostream>\n\nint main() {\n    std::cout << \"Hello, " + project + "!\" << std::endl;\n    return 0;\n}"
    );

    // Makefile with explicit tabs to prevent errors
    create_file(project + "/Makefile",
        "CXX = g++\n"
        "CXXFLAGS = -Wall -Iinclude -g\n"
        "SRC = $(wildcard src/*.cpp)\n"
        "OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))\n"
        "BIN = bin/" + project + "\n\n"
        "all: $(BIN)\n\n"
        "$(BIN): $(OBJ)\n"
        "	mkdir -p bin\n"
        "	$(CXX) $(CXXFLAGS) -o $@ $^\n\n"
        "build/%.o: src/%.cpp\n"
        "	mkdir -p build\n"
        "	$(CXX) $(CXXFLAGS) -c $< -o $@\n\n"
        "test: tests/test_math.cpp\n"
        "	mkdir -p bin\n"
        "	$(CXX) $(CXXFLAGS) -Itests -o bin/test_math tests/test_math.cpp\n"
        "	./bin/test_math\n\n"
        "clean:\n"
        "	rm -rf build bin\n"
    );

    // test_math.cpp
    create_file(project + "/tests/test_math.cpp",
        "#include <cassert>\n#include <iostream>\n\nint main() {\n    assert(2 + 2 == 4);\n    std::cout << \"All tests passed!\\n\";\n    return 0;\n}"
    );

    // .gitignore
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

    // README.md
    create_file(project + "/README.md", "# " + project + R"(

This is an automatically generated C++ project.

## Build

```bash
make
```

## Run

```bash
./bin/)" + project + R"(
```

## Run tests

```bash
make test
```

## Clean build files

```bash
make clean
```
)");

    // Initialize Git repo if flag is passed
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
    return 0;
}
