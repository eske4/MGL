add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/configured_files/include")
target_link_libraries(${PROJECT_NAME} PUBLIC Lexer)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/app)
