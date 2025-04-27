set(GENERATED_ASM_DIR "${CMAKE_SOURCE_DIR}/assembly/generated/")

configure_file("config.c.in" "${CMAKE_BINARY_DIR}/configured_files/include/config.c" ESCAPE_QUOTES)
configure_file(
  "assembly.c.in" "${CMAKE_BINARY_DIR}/configured_files/include/assembly.c" ESCAPE_QUOTES
)
