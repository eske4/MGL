set(LEXER_SOURCES lexer.c)

add_library(Lexer ${LEXER_SOURCES})

target_include_directories(Lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Lexer Compiler_State Error_Handler)
