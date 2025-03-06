enable_testing()

add_executable(example_test example.cpp)
target_link_libraries(example_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(example_test)
