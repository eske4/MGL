# Lexer tests
add_executable(lexer_test lexer_test.cpp)
target_include_directories(lexer_test PUBLIC ${CMAKE_BINARY_DIR}/configured_files/include)
target_link_libraries(lexer_test PRIVATE GTest::gtest_main Lexer)
gtest_discover_tests(lexer_test)

# Parser tests
add_executable(parser_test parser_test.cpp)
target_compile_definitions(parser_test PRIVATE PARSER_UNIT_TEST)

target_link_libraries(parser_test PRIVATE GTest::gtest_main Parser Lexer Error_Handler)
gtest_discover_tests(parser_test)

# Type-Cheker tests
add_executable(type_checker_test type_checker_test.cpp)

target_link_libraries(
  type_checker_test PRIVATE GTest::gtest_main Semantic_Analyzer Parser Common_Headers Error_Handler
)
gtest_discover_tests(type_checker_test)

# IL tests
add_executable(il_test il_test.cpp)

target_link_libraries(il_test PRIVATE GTest::gtest_main Intermediate_Language Parser Common_Headers)
gtest_discover_tests(il_test)
