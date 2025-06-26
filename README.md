# CPP Starter - CPP Project Generator CLI

`cppstarter` is a simple command-line tool to quickly scaffold C++ projects with a ready-to-use Makefile, directory structure, test setup, and optional Git initialization.

## Features

- Creates standard C++ project directories (`src`, `include`, `tests`, `bin`, `build`)
- Generates a Makefile with build, test, and clean targets
- Includes sample `main.cpp` and simple test example
- Creates `.gitignore` and `README.md` files automatically
- Optional Git repository initialization with `--init-git` flag

## Installation

### Compile the generator

```bash
g++ cppstarter.cpp -o cppstarter
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

Generate a new project:

```bash
./cppstarter new MyProject
```

If installed globally:

```bash
cppstarter new MyProject
```

Generate a new project and initialize a Git repository:

```bash
./cppstarter new MyProject --init-git
```

or if installed globally:

```bash
cppstarter new MyProject --init-git
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
