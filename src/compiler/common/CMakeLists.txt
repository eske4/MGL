add_library(Compiler_State STATIC compiler_state.c)
add_library(Error_Handler STATIC error_handler.c)
add_library(Common_Headers INTERFACE)

target_include_directories(Common_Headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  Compiler_State PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include # Add this to expose the
                        # header
)

target_include_directories(
  Error_Handler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include # Add this to expose the
                       # header
)
