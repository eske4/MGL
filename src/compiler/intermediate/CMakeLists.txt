set(IRGENERATOR_SOURCES STATIC ir_generator.c)

add_library(IRGenerator ${IRGENERATOR_SOURCES})

target_include_directories(
    IRGenerator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${COMPILER_COMMON_INCLUDE}
)


target_link_libraries(IRGenerator PRIVATE Lexer Parser)
