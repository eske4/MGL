set(ASM_DIR "${CMAKE_SOURCE_DIR}/assembly/generated/")
set(SOURCE_CODE_FILE "${CMAKE_SOURCE_DIR}/data/input.MF")
set(GENERATED_DIR "${CMAKE_SOURCE_DIR}/generated/")
set(TEST_FILE_DIR "${CMAKE_SOURCE_DIR}/tests/files/")
set(INTEGRATION_TEST_FILE "${CMAKE_SOURCE_DIR}/tests/integration_tests/data/input_test.MF")

configure_file("config.c.in" "${CMAKE_BINARY_DIR}/configured_files/include/config.c" ESCAPE_QUOTES)
configure_file(
  "file_config.c.in" "${CMAKE_BINARY_DIR}/configured_files/include/file_config.c" ESCAPE_QUOTES
)
