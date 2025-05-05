add_executable(integration_tests integration_tests.cpp)

target_include_directories(integration_tests PUBLIC "${CMAKE_BINARY_DIR}/configured_files/include")

target_link_libraries(integration_tests GTest::gtest_main Lexer Compiler_State Error_Handler Parser Type_Checker Intermediate_Language)

include(GoogleTest)
gtest_discover_tests(integration_tests)