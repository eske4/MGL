# Create a static library for Error_Handler
add_library(Error_Handler STATIC ${CMAKE_CURRENT_SOURCE_DIR}/error_handler.cpp)

# Optionally, specify include directories for Error_Handler (if needed)
target_include_directories(Error_Handler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
