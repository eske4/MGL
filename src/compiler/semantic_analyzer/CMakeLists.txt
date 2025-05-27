set(SEMANTIC_ANALYZER_SOURCES semantic_analyzer.c look_up_tables.c)

add_library(Semantic_Analyzer ${SEMANTIC_ANALYZER_SOURCES})

target_include_directories(Semantic_Analyzer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(Semantic_Analyzer PRIVATE Parser Common_Headers)
