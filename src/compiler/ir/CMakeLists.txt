set(IR_SOURCES ir.c instr_table.c)

add_library(IR ${IR_SOURCES})

target_include_directories(IR PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(IR PRIVATE Parser Compiler_State)
