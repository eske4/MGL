cmake_minimum_required(VERSION 3.25.1)

project(
  mgl
  VERSION 1.0.0
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # auto complete for nvim

include(cmake/CPM.cmake)
include(cmake/Packages.cmake)

add_subdirectory(configured)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(app)
