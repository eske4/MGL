# MGL - Map Generation Language
Computer Science Master's Project (Second Semester)

**MGL** is a simple domain-specific language (DSL) for defining and compiling graph-based map structures. It was developed as a university project using C/C++, with inspiration drawn from how maps are represented in computer games.

## Installation & Setup

### System Requirements
| Component      | Linux(Arch, Ubuntu)                  | macOS                  | Windows (WSL2)        |
|---------------|------------------------|------------------------|-----------------------|
| **Compiler**  | GCC ≥ 14.2.1           | GCC 16+              | GCC via Ubuntu/Arch   |
| **Assembler**  | NASM ≥ 2.16.03          | NASM ≥ 2.16.03       | NASM via Ubuntu/Arch   |
| **Build Tools**| CMake ≥ 3.25.1         | CMake ≥ 3.25.1         | CMake ≥ 3.25.1        |
|               | GNU Make ≥ 4.4.1         | GNU Make ≥ 4.4.1       | GNU Make ≥ 4.4.1        |
| **Demo Tools**  | Python ≥ 3.13.3         | Python ≥ 3.13.3       | Python ≥ 3.13.3        |
| **Environment**| -                      | Ventura+ recommended   | WSL2 with Ubuntu/Arch |

*Note: All platforms require standard C23/C++23 toolchain support.*

### Building from Source
1. git clone https://github.com/eske4/MGL.git
2. cd MGL
3. make build

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
     * **Map declaration**: `Map mapname { ... }`
     * **Rooms**: `Room RoomName;`
     * **Connections**:  
       - `->` for one-way  
       - `<->` for two-way  
     * End all statements with a semicolon (`;`)
     * See `data/input_example.MF` for complete examples.

2. **Generate assembly**  
   To generate assembly code:  
   `make generate`

3. **Validate map**  
   Run the validation process:  
   `make run`  
   Validator checks:
   * All rooms are traversable.
  
4. **Use Generated Assembly**  
   The assembly code is saved to `data/map.asm`. This file can now be used in other applications as its assembly representation.  
   You can integrate this `map.asm` file into tools that supports assembly input. For more information, refer to the integration section.

## Command Reference

| Command          | Description                                                  |
|------------------|--------------------------------------------------------------|
| `make`           | Runs the full pipeline (build, test, and generate)            |
| `make build`     | Compiles all C/C++ source files                              |
| `make generate`  | Converts `input.MF` to assembly code                         |
| `make run`       | Executes a map validation algorithm                        |
| `make test`      | Runs unit and integration tests                              |
| `make clean`     | Removes all build artifacts                                  |
| `make py_demo`   | Runs a Python integration demo (NASM assembly linked with Python) |
| `make c_demo`    | Runs a C integration demo (NASM assembly linked with C)      |

## Testing
Run the verification suite:
make test

## Maintenance
Clean build artifacts:
make clean

## Troubleshooting
- Map generation errors: Check data/input.MF syntax
- Build issues: Verify compiler versions support C++23/C23
- Runtime problems: Ensure WSL 2 is updated if on Windows

## Integration

Example integrations using **C** and **Python** can be found in the `usage/` directory. These demos show how to link the generated `map.asm` file into a working program.

### Run the Examples

To quickly build and run the demos, use the provided Makefile (all can be done from the root project):

```bash
make c_demo    # Build and run the C demo
make py_demo   # Build and run the Python demo
```

### Manual Compilation

Prefer to build it yourself? Here's how to compile everything manually from the `usage/` directory:

#### 1. Linux:

C Integration
```bash
nasm -f elf64 map.asm -o map.o
gcc demo.c map.o -o demo
./demo
```


Python Integration
```bash
nasm -f elf64 map.asm -o map.o
gcc -shared map.o -o map.so
python demo.py
```

#### 2. Mac:
   ...

## Original authors:
1. [Eske Klintø] - [eske.larsen@gmail.com]
2. ...

