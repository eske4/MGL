set(IL_SOURCES il.c instr_table.c)

add_library(Intermediate_Language ${IL_SOURCES})

target_include_directories(Intermediate_Language PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Intermediate_Language PRIVATE Parser Common_Headers)
