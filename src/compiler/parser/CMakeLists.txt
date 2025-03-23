# Specify the source and header files for Bison
set(BISON_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.y)

# Include any necessary header files for the parser
set(BISON_INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/parser.h)

# Define the Bison target to generate parser.c and parser.h
bison_target(
  Parser ${BISON_FILE} ${CMAKE_CURRENT_BINARY_DIR}/parser.c
  DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
)

# Add the parser as a static library
add_library(Parser STATIC ${BISON_Parser_OUTPUTS} ${BISON_INCLUDE_FILES})

# Make sure to include the necessary directories for the Bison generated files
target_include_directories(
  Parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include # Include folder for custom headers like parse.h
                ${CMAKE_CURRENT_BINARY_DIR} # Include folder for the generated files (parser.h)
)

# Link the LexerWBison library (ensure that LexerWBison is already built elsewhere)
target_link_libraries(Parser PRIVATE LexerWBison)

# If needed, you can pass the output directory of the generated Bison files to the parent scope
set(BISON_DIR
    ${CMAKE_CURRENT_BINARY_DIR}
    PARENT_SCOPE
)
