set(PARSER_SOURCES parser.c astree.c declaration_tracker.c)

add_library(Parser ${PARSER_SOURCES})
target_include_directories(
  Parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include # Add this to expose the
)
target_link_libraries(Parser PRIVATE Lexer)
