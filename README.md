# MGL - Map Generation Language
Computer Science Master's Project (Second Semester)

A domain-specific language for procedural map generation with C/C++ compilation pipeline.

## Installation & Setup

### Requirements
- Supported Platforms:
  - Linux (Ubuntu, Arch Linux)
  - macOS (Ventura+ recommended)
  - Windows (via WSL 2 with Ubuntu or Arch)

- Toolchain:
  - C++23 compatible compiler (GCC 13+ or Clang 16+)
  - C23 compatible compiler
  - CMake >= 3.25.1
  - GNU Make

### Building from Source
- git clone https://github.com/eske4/MGL.git
- cd MGL
- make build

## Usage Workflow

1. Design your map:
   - Create/edit data/input.MF using the MGL syntax

2. Generate assembly:
   make generate

3. Validate map:
   make run

## Command Reference

| Command          | Description                                  |
|-----------------|---------------------------------------------|
| make            | Full pipeline (build -> test -> generate)    |
| make build      | Compiles all C/C++ sources                  |
| make generate   | Converts input.MF to assembly               |
| make run        | Executes and validates generated map        |
| make test       | Runs verification tests                     |
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

## License and Usage
This project is available for academic and research purposes. For any other use, please contact the maintainers for permission. The source code and documentation are provided as-is without warranty of any kind.

## Contact
For usage permissions or questions:
- Maintainer: Eske Klintø Larsen
- Email: eske.larsen@gmail.com


