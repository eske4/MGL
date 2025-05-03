# add_executable(unit_test lexer_test.cpp parser_test.cpp)

# target_include_directories(
#   unit_test
#   PUBLIC ${CMAKE_BINARY_DIR}/configured_files/include
# )

# target_link_libraries(unit_test GTest::gtest_main Lexer Parser Compiler_State Common_Headers)

# include(GoogleTest)
# gtest_discover_tests(unit_test)

# Lexer‐only tests
add_executable(lexer_test lexer_test.cpp)
target_include_directories(
  lexer_test
  PUBLIC ${CMAKE_BINARY_DIR}/configured_files/include
)
target_link_libraries(lexer_test PRIVATE
  GTest::gtest_main
  Lexer
  Common_Headers
  Compiler_State
)
include(GoogleTest)
gtest_discover_tests(lexer_test)

# Parser‐only tests
add_executable(parser_test parser_test.cpp)
target_compile_definitions(parser_test PRIVATE PARSER_UNIT_TEST)
target_include_directories(
  parser_test
  PUBLIC ${CMAKE_BINARY_DIR}/configured_files/include
)
target_link_libraries(parser_test PRIVATE
  GTest::gtest_main
  Lexer
  Parser
  Common_Headers
  Compiler_State
)
include(GoogleTest)
gtest_discover_tests(parser_test)
