add_executable(lexer_test lexer_test.cpp)
target_link_libraries(lexer_test GTest::gtest_main Lexer)

include(GoogleTest)
gtest_discover_tests(lexer_test)
