# Build artifacts
MAP_OBJ = map.o
MAP_SO = map.so
C_DEMO = c_demo

# Phony targets (don't create files with these names)
.PHONY: all clean compile py_demo c_demo


all: compile py_demo c_demo

build:
	@nasm -f elf64 map.asm -o map.o
	@gcc -shared map.o -o map.so
	@gcc demo.c map.o -o c_demo

py_demo:
	@python demo.py

c_demo:
	@./$(C_DEMO)

clean:
	@rm -f $(MAP_OBJ) $(MAP_SO) $(C_DEMO)
