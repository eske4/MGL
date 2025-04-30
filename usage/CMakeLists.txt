set(CMAKE_ASM_NASM_FLAGS "-f elf64")

add_library(map_object OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/map.asm")

add_executable(demo demo.c)

target_link_libraries(demo PRIVATE map_object)
