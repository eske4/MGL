# MGL - Map Generation Language

Computer Science Master's Project (Second Semester)

**MGL** is a simple domain-specific language (DSL) for defining and compiling graph-based map structures. It was developed as a university project using C/C++, with inspiration drawn from how maps are represented in computer games.


---

## Installation & Setup

### System Requirements

| Component       | Linux(Arch, Ubuntu) | macOS                  | Windows (WSL2)        |
| --------------- | ------------------- | ---------------------- | --------------------- |
| **Compiler**    | GCC ≥ 14.2.1        | CLANG 16+              | GCC via Ubuntu/Arch   |
| **Assembler**   | NASM ≥ 2.16.03      | NASM ≥ 2.16.03         | NASM via Ubuntu/Arch  |
| **Build Tools** | CMake ≥ 3.25.1      | CMake ≥ 3.25.1         | CMake ≥ 3.25.1        |
|                 | GNU Make ≥ 4.4.1    | GNU Make ≥ 4.4.1       | GNU Make ≥ 4.4.1      |
| **Demo Tools**  | Python ≥ 3.13.3     | Python Anaconda ≥ 3.10 | Python ≥ 3.13.3       |
| **Environment** | -                   | Sequoia Recommended    | WSL2 with Ubuntu/Arch |

_Note: All platforms require standard C23/C++23 toolchain support._

---

### Building from Source

1. git clone https://github.com/eske4/MGL.git
2. cd MGL
3. make build

---

## Usage Workflow

1. **Design your map**

   - File location: `data/input.MF`
   - Basic syntax:
     ```
     Map Example {
         Room A;
         Room B;
         Connect(A -> B);
     }
     ```
   - Key rules:
     - **Map declaration**: `Map mapname { ... }`
     - **Rooms**: `Room RoomName;`
     - **Connections**:
       - `->` for one-way
       - `<->` for two-way
     - End all statements with a semicolon (`;`)
     - See `data/input_example.MF` for complete examples.

2. **Generate assembly**  
   To generate assembly code:  
   `make generate`

### 3. Validate the Map

```bash
make run
```

The validator checks:
- All rooms are traversable


Note: Windows users must use WSL2 to run this.

### 4. Use Generated Assembly

- Output: `data/map.asm`
- Can be embedded in C, Python, or other tools supporting assembly


---

## Command Reference

| Command         | Description                                                       |
|----------------|-------------------------------------------------------------------|
| `make`          | Full pipeline: (build -> test -> generate)                        |
| `make build`    | Compile all C/C++ source files                                    |
| `make generate` | Convert `.MF` map into assembly code                              |
| `make run`      | Validate map traversal and integrity                              |
| `make test`     | Run unit and integration tests                                    |
| `make clean`    | Remove all build artifacts                                        |
| `make py_demo`  | Run Python integration demo (NASM + Python)                       |
| `make c_demo`   | Run C integration demo (NASM + C)                                 |

---

## Testing

Run the verification suite:
make test

---

## Maintenance

Clean up build artifacts:

```bash
make clean
```

---

## Troubleshooting

- **Map generation error**: Check `data/input.MF` syntax
- **Build issues**: Confirm compiler supports C++23/C23
- **WSL2 users**: Make sure it's updated and properly configured


---

## Integration

You’ll find example C and Python integrations under `usage/`.


### Quick Demos

```bash
make c_demo    # C integration
make py_demo   # Python integration
```
**Note:** Win only support quickdemo on WSL2

**Note:** On macOS, the Python demo requires setting up a Conda Enviroment [See Conda Setup](#conda-setup). If already done before running quickdemo `conda activate my_x86_env` 

### Manual Compilation

Prefer to build it yourself? Here's how to compile everything manually from the `usage/` directory:

#### Linux

**C:**

```bash
nasm -f elf64 map.asm -o map.o
gcc demo.c map.o -o demo
./demo
```

**Python:**

```bash
nasm -f elf64 map.asm -o map.o
gcc -shared map.o -o map.so
python demo.py
```


#### macOS

**C:**

```bash
nasm -f macho64 map_mac.asm -o map.o
arch -x86_64 clang demo.c map.o -o c_demo
./c_demo
```

**Conda Setup**
```bash
conda create -n my_x86_env -y
conda activate my_x86_env
conda config --env --set subdir osx-64
conda install python=3.10
```

**Python (with Conda):**

```bash
conda activate my_x86_env

nasm -f macho64 map_mac.asm -o map.o
arch -x86_64 clang -shared map.o -o map.so
python demo.py
```

#### Windows

Use WSL2 and follow Linux instructions.

Alternatively, for native Windows builds:

1. Install Chocolatey
2. Use Chocolatey to install MSYS2
3. Add to PATH: `C:\tools\msys64\mingw64\bin`
4. In MSYS2 terminal:
   ```bash
   pacman -S make gcc cmake nasm python
   ```

Then:

```cmd
winmake.bat build
winmake.bat c_demo
winmake.bat py_demo
```

If Python demo fails, ensure MSYS2’s Python is being used, not the system default.

---


## Original authors:

1. [Eske Klintø] - [eske.larsen@gmail.com]
2. [Faustas Anulis] - [faustis1337@gmail.com]
