add_executable(${PROJECT_NAME} main.c)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_BINARY_DIR}/configured_files/include")
target_link_libraries(${PROJECT_NAME} Compiler)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/app)
