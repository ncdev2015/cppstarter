# CPP Starter - CPP Project Generator CLI

`cppstarter` is a simple command-line tool to quickly scaffold C++ projects with a ready-to-use Makefile, directory structure, test setup, and optional Git initialization.

## Features

- Creates standard C++ project directories (`src`, `include`, `tests`, `build`)
- Generates a Makefile with build, debugging, test and clean targets
- Includes sample `main.cpp` and simple test example
- Creates `.gitignore` and `README.md` files automatically
- Optional Git repository initialization with `--init-git` flag
- Includes helper commands: `--help`, `--version`, `run`, `run-release`, `test`, `valgrind` and `min`

## Installation

### Option 1: Download Pre-built Binary
Download the latest binary from the [Releases](https://github.com/ncdev2015/cppstarter/releases) section on GitHub.

### Option 2: Compile from Source

#### Compile the generator (debug build)
```bash
make
```

#### Compile the generator for release
```bash
make release
```

#### See all available build targets
```bash
make help
```

#### (Optional) Install globally (Linux/macOS)
After building the project (in debug or release mode), install the cppstarter binary system-wide by running the install command from the root of the project:

```bash
sudo make install
```

By default, this installs to `/usr/local/bin`. You can specify a custom installation prefix:

```bash
sudo make install PREFIX=/custom/path
```

Make sure `/usr/local/bin` (or your custom path) is in your `PATH` environment variable:

```bash
echo $PATH
```

If you don't see `/usr/local/bin`, add it by editing your shell config file (e.g., `~/.bashrc` or `~/.zshrc`):

```bash
export PATH=$PATH:/usr/local/bin
```

Then reload the shell:

```bash
source ~/.bashrc  # or ~/.zshrc
```

## Usage

### Show help
```bash
cppstarter --help
```

### Show version
```bash
cppstarter --version
```

### Create a new project
```bash
cppstarter new MyProject
```

### Create a new project and initialize Git
```bash
cppstarter new MyProject --init-git
```

### Run the project (debug build with colored output)
```bash
cd MyProject
cppstarter run
```

### Run the release build
```bash
cd MyProject
cppstarter run-release
```

If the executable is not found, a helpful error message will suggest compiling the project first.

### Compact the terminal prompt
```bash
cppstarter min
source min.sh
```

## Development

### Available Make Targets

- `make` or `make all` - Build debug version (default)
- `make release` - Build optimized release version
- `make test` - Compile and run tests
- `make run` - Run application in debug mode (with colored output)
- `make run-release` - Run application in release mode
- `make valgrind` - Run debug application with valgrind
- `make install` - Install release binary to system (default: /usr/local/bin)
- `make clean` - Remove all compiled files and directories
- `make help` - Show help with all available targets

### Testing with Valgrind
```bash
make valgrind
```

## Generated Project Structure

```
MyProject/
├── Makefile
├── README.md
├── .gitignore
├── src/
│   └── main.cpp
├── include/
├── tests/
│   └── test_math.cpp
└── build/
```

## License

MIT License