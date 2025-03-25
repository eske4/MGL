add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Parser Lexer)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/configured_files/include")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/app)
