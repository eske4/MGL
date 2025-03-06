add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC Lexer)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/app)
