add_executable(parser_test parser_test.cpp)
target_link_libraries(parser_test GTest::gtest_main Error_Handler Parser Lexer)
target_compile_definitions(parser_test PRIVATE __GTEST__)

include(GoogleTest)
gtest_discover_tests(parser_test)
