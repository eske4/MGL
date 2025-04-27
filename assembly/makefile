run:
	@nasm -f elf64 -DLINUX x86-64_main.asm -o main.o
	@ld main.o -o main.out
	@./main.out

clean:
	@rm -f main.o main.out
