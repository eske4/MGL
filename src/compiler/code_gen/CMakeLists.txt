set(CODE_GEN_SOURCES gen_linux_asm.c)

add_library(Code_Gen ${CODE_GEN_SOURCES})

target_include_directories(Code_Gen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Code_Gen PRIVATE Intermediate_Language Common_Headers)
