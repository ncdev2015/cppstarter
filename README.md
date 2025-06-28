# CPP Starter - CPP Project Generator CLI

`cppstarter` is a simple command-line tool to quickly scaffold C++ projects with a ready-to-use Makefile, directory structure, test setup, and optional Git initialization.

## Features

- Creates standard C++ project directories (`src`, `include`, `tests`, `build`)
- Generates a Makefile with build, test, and clean targets
- Includes sample `main.cpp` and simple test example
- Creates `.gitignore` and `README.md` files automatically
- Optional Git repository initialization with `--init-git` flag
- Includes helper commands: `--help`, `--version`, `run`, `run`, `run-release` and `min`

## Installation

### Compile the generator

```bash
make
```

### Compile the generator for release

```bash
make release
```

### (Optional) Install globally (Linux/macOS)

To make `cppstarter` available system-wide, move it to `/usr/local/bin` and give execution permissions:

```bash
sudo mv cppstarter /usr/local/bin/
sudo chmod +x /usr/local/bin/cppstarter
```

Make sure `/usr/local/bin` is in your `PATH` environment variable:

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

### Run the project (debug build)

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
