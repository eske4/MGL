cmake_minimum_required(VERSION 3.25.1)

project(
  mgl
  VERSION 1.0.0
  LANGUAGES C CXX ASM_NASM
  DESCRIPTION "A domain-specific language compiler for generating graph-based maps"
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # auto complete for nvim

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")

include(Build)
include(CPM)
include(Packages)

add_subdirectory(configured)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(app)
