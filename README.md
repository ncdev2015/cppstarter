# CPP Starter - CPP Project Generator CLI

`cppstarter` is a simple command-line tool to quickly scaffold C++ projects with a ready-to-use Makefile, directory structure, test setup, and optional Git initialization.

## Features

- Creates standard C++ project directories (`src`, `include`, `tests`, `bin`, `build`)
- Generates a Makefile with build, test, and clean targets
- Includes sample `main.cpp` and simple test example
- Creates `.gitignore` and `README.md` files automatically
- Optional Git repository initialization with `--init-git` flag

## Installation

Compile the generator:

```bash
g++ cppstarter.cpp -o cppstarter
```

## Usage

Generate a new project:

```bash
./cppstarter new MyProject
```

Generate a new project and initialize a Git repository:

```bash
./cppstarter new MyProject --init-git
```

### Install globally (Linux/macOS)

To make `cppstarter` available system-wide:

```bash
sudo mv cppstarter /usr/local/bin
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
├── bin/
└── build/
```

## License

MIT License
