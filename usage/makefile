compile:
	nasm -f elf64 map.asm -o map.o
	gcc -shared map.o -o map.so

py_demo:
	python demo.py

c_demo:
	gcc demo.c map.o -o c_demo
