# Detect the OS
# 		@$(LINK_CMD)
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
    ASM_FORMAT := elf64
    ASM_FLAGS := -DLINUX
    LINK_CMD := ld main.o -o main.out
    RUN_CMD := ./main.out
else ifeq ($(UNAME_S), Darwin)
    ASM_FORMAT := macho64
    ASM_FLAGS := -DMAC
    LINK_CMD := clang -arch x86_64 -Wl,-e,_start -Wl, -o main.out main.o
    RUN_CMD := arch -x86_64 ./main.out
else
    $(error Unsupported OS: $(UNAME_S))
endif

run:
	@nasm -f $(ASM_FORMAT) $(ASM_FLAGS) x86-64_main.asm -o main.o
	@$(LINK_CMD)
	@$(RUN_CMD)

clean:
	@rm -f main.o main.out generated/map.asm generated/config.asm
