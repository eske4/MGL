add_library(Compiler_State STATIC compiler_state.c)
add_library(Error_Handler STATIC error_handler.cpp)

target_include_directories(
  Compiler_State
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include # Add this to expose the
         # header
)

target_include_directories(
  Error_Handler
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include # Add this to expose the
         # header
)
