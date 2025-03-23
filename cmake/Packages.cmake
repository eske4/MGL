message("Fetching packages")

CPMAddPackage("gh:google/googletest@1.15.2")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
