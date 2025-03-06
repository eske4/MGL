cmake_minimum_required(VERSION 3.25.1)

project(mgl)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True)

include(cmake/CPM.cmake)

cpmaddpackage("gh:google/googletest@1.15.2")

add_subdirectory(tests)
add_subdirectory(src)
add_subdirectory(app)
