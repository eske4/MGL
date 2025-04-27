run:
	@nasm -f elf64 main.asm -o main.o
	@ld main.o -o main.out
	@./main.out

clean:
	@rm -f main.o main.out
