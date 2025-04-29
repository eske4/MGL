# MGL - Map Generation Language
Computer Science Master's Project (Second Semester)

**MGL** is a simple domain-specific language (DSL) for defining and compiling graph-based map structures. It was developed as a university project using C/C++, with inspiration drawn from how maps are represented in computer games.

## Installation & Setup

### Requirements
- Supported Platforms:
  - Linux (Arch Linux, Ubuntu)
  - macOS (Ventura+ recommended)
  - Windows (via WSL 2 with Ubuntu or Arch)

- Toolchain:
  - C++23 compatible compiler (GCC 13+ or Clang 16+)
  - C23 compatible compiler
  - CMake >= 3.25.1
  - GNU Make

### Building from Source
1. git clone https://github.com/eske4/MGL.git
2. cd MGL
3. make build

## Usage Workflow

1. Design your map:
   - File location: data/input.MF
   - Basic syntax:
Map example {
    Room A;
    Room B;
    Connect(A -> B);
}

   - Key rules:
     * Map declaration: Map mapname { ... }
     * Rooms: Room RoomName;
     * Connections: 
       - -> for one-way
       - <-> for two-way
     * End all statements with ;
     * See data/input_example.MF for complete examples

2. Generate assembly:
make generate

3. Validate map:
make run
   - Validator checks:
     * All rooms are traversible

## Command Reference

| Command          | Description                                  |
|-----------------|---------------------------------------------|
| make            | Full pipeline (build -> test -> generate)    |
| make build      | Compiles all C/C++ sources                  |
| make generate   | Converts input.MF to assembly               |
| make run        | Executes and validates generated map        |
| make test       | Runs unit and integration tests                     |
| make clean      | Removes build artifacts                     |

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


## Original authors:
1. [Eske Klintø] - [eske.larsen@gmail.com]
2. ...

