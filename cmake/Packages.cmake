message("Fetching packages")

CPMAddPackage("gh:google/googletest@1.15.2")
find_package(FLEX 2.6.4 EXACT REQUIRED)
find_package(BISON 3.8.2 EXACT REQUIRED)

message(STATUS "FLEX version: ${FLEX_VERSION}")
message(STATUS "BISON version: ${BISON_VERSION}")

if(NOT FLEX_FOUND)
  message(
    FATAL_ERROR
      "Flex is required for this project. Please install Flex.\n"
      "On Linux: sudo apt install flex\n" "On macOS: brew install flex\n"
      "On Windows: use MSYS2 or WSL to install Flex."
  )
endif()

if(NOT BISON_FOUND)
  message(
    FATAL_ERROR
      "Bison is required for this project. Please install Bison.\n"
      "On Linux: sudo apt install bison\n" "On macOS: brew install bison\n"
      "On Windows: use MSYS2 or WSL to install Bison."
  )
endif()
