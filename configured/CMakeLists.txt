set(GENERATED_ASM_DIR "${CMAKE_SOURCE_DIR}/assembly/generated/")
set(SOURCE_CODE_FILE "${CMAKE_SOURCE_DIR}/data/input.MF")

configure_file("config.c.in" "${CMAKE_BINARY_DIR}/configured_files/include/config.c" ESCAPE_QUOTES)
configure_file(
  "file_config.c.in" "${CMAKE_BINARY_DIR}/configured_files/include/file_config.c" ESCAPE_QUOTES
)
