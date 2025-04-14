set(TYPE_CHECKER_SOURCES type_checker.c look_up_tables.c)

add_library(Type_Checker ${TYPE_CHECKER_SOURCES})

target_include_directories(
  Type_Checker
  PRIVATE ${COMPILER_COMMON_INCLUDE}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Type_Checker PRIVATE Parser Common_Headers)
