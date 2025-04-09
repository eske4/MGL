set(TYPE_CHECKER_SOURCES type_checker.c)

add_library(TypeChecker ${TYPE_CHECKER_SOURCES})
target_include_directories(
  TypeChecker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${COMPILER_COMMON_INCLUDE}
)

target_link_libraries(TypeChecker PRIVATE Lexer Parser Compiler_State)
