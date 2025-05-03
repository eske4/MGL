UNAME_S := $(shell uname -s)

# Build artifacts
MAP_OBJ = map.o
MAP_SO = map.so
C_DEMO = c_demo

# Phony targets (don't create files with these names)
.PHONY: all clean compile py_demo c_demo

all: build py_demo c_demo

build:
ifeq ($(UNAME_S), Linux)
	@nasm -f elf64 map.asm -o map.o
	@gcc -shared map.o -o map.so
	@gcc demo.c mgl_interface.h map.o -o c_demo
else ifeq ($(UNAME_S), Darwin)
	@nasm -f macho64 map_mac.asm -o map.o
	@arch -x86_64 clang -shared map.o -o map.so
	@arch -x86_64 clang demo.c mgl_interface.h map.o -o c_demo
	@echo "Make sure you have Conda environment installed to run python script! Reference to README 'Python Environment Setup'"
else
	$(error USAGE demos: Unsupported OS: $(UNAME_S))
endif

py_demo:
	@python demo.py

c_demo:
	@./$(C_DEMO)

clean:
	@rm -f $(MAP_OBJ) $(MAP_SO) $(C_DEMO)
