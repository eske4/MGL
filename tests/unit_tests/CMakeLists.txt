# Lexer‐only tests
add_executable(lexer_test lexer_test.cpp)
target_include_directories(
  lexer_test
  PUBLIC ${CMAKE_BINARY_DIR}/configured_files/include
)
target_link_libraries(lexer_test PRIVATE
  GTest::gtest_main
  Lexer
)
gtest_discover_tests(lexer_test)

# Parser‐only tests
add_executable(parser_test parser_test.cpp)
target_compile_definitions(parser_test PRIVATE PARSER_UNIT_TEST)

target_link_libraries(parser_test PRIVATE
  GTest::gtest_main
  Parser
  Lexer
  Error_Handler
)
gtest_discover_tests(parser_test)

# Type-Cheker-only tests
add_executable(type_checker_test type_checker_test.cpp)

target_link_libraries(type_checker_test PRIVATE
  GTest::gtest_main
  Type_Checker
  Parser
  Common_Headers
  Error_Handler
)
gtest_discover_tests(type_checker_test)
