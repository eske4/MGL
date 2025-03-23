enable_testing()

add_executable(lexer_test lexer_test.cpp)
add_executable(parser_test parser_test.cpp)
target_link_libraries(lexer_test GTest::gtest_main Lexer)
target_link_libraries(parser_test GTest::gtest_main Parser LexerWBison)

include(GoogleTest)
gtest_discover_tests(lexer_test)
gtest_discover_tests(parser_test)
