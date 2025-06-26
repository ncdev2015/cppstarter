#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstdlib>

namespace fs = std::filesystem;

void create_file(const fs::path& path, const std::string& content) {
    std::ofstream file(path);
    if (file) file << content;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || std::string(argv[1]) != "new") {
        std::cerr << "Usage: " << argv[0] << " new <ProjectName> [--init-git]\n";
        return 1;
    }

    std::string project = argv[2];
    bool init_git = (argc == 4 && std::string(argv[3]) == "--init-git");

    // Create directories
    fs::create_directories(project + "/src");
    fs::create_directories(project + "/include");
    fs::create_directories(project + "/tests");
    fs::create_directories(project + "/bin");
    fs::create_directories(project + "/build");

    // main.cpp
    create_file(project + "/src/main.cpp",
        "#include <iostream>\n\nint main() {\n    std::cout << \"Hello, " + project + "!\" << std::endl;\n    return 0;\n}"
    );

    // Makefile
    create_file(project + "/Makefile", R"(CXX = g++
CXXFLAGS = -Wall -Iinclude -g
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
BIN = bin/)" + project + R"(

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: tests/test_math.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -Itests -o bin/test_math tests/test_math.cpp
	./bin/test_math

clean:
	rm -rf build bin
)"
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
        std::system(cmd.c_str());
    }

    std::cout << "Project '" << project << "' created successfully.\n";
    if (init_git) {
        std::cout << "Git repository initialized.\n";
    }

    return 0;
}
