set(LIBRARY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lexer.c)
set(LIBRARY_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/lexer.h)

# Create the Microphone library
add_library(Lexer ${LIBRARY_SOURCES} ${LIBRARY_HEADER})

target_include_directories(Lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
