add_subdirectory(common)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(type_checker)
add_subdirectory(intermediate_lang)
add_subdirectory(code_gen)

set(PARSER_SOURCES compiler.c)

add_library(Compiler ${PARSER_SOURCES})
target_include_directories(Compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Compiler PRIVATE Lexer Parser Type_Checker Intermediate_Language Code_Gen)
