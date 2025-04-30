if(APPLE)
  set(NASM_FORMAT "macho64")
elseif(WIN32)
  set(NASM_FORMAT "win64")
else() # Assume Linux/Unix by default
  set(NASM_FORMAT "elf64")
endif()

set(CMAKE_ASM_NASM_FLAGS "-f ${NASM_FORMAT}")

add_library(map_object OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/map.asm")

add_executable(demo demo.c)

target_link_libraries(demo PRIVATE map_object)
