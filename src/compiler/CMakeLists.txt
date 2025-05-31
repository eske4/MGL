add_subdirectory(common)
add_subdirectory(lexer)
add_subdirectory(parser)
add_subdirectory(semantic_analyzer)
add_subdirectory(intermediate_lang)
add_subdirectory(code_gen)

set(COMPILER_SOURCES compiler.c)

add_library(Compiler ${COMPILER_SOURCES})
target_include_directories(Compiler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
  Compiler PRIVATE Lexer Parser Semantic_Analyzer Intermediate_Language Code_Gen
)
