# Set source and header files
set(FLEX_FILE ${CMAKE_CURRENT_SOURCE_DIR}/lexer.l)

# Correctly declare the header files to include in the library
set(FLEX_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/lexer.h)

flex_target(Lexer ${FLEX_FILE} ${CMAKE_CURRENT_BINARY_DIR}/lexer.c COMPILE_FLAGS "-d")

add_library(Lexer STATIC ${FLEX_Lexer_OUTPUTS} ${FLEX_INCLUDE_FILES})

target_include_directories(Lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${BISON_DIR})
target_link_libraries(Lexer PRIVATE Parser Error_Handler)

add_dependencies(Lexer Parser)
