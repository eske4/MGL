#include <gtest/gtest.h>
#include "parser.h"

#define PARSER_SUCCEEDED 0
#define PARSER_FAILED 1
#define MEMORY_FAILED 2

// Testing valid tokens like keywords and literals


TEST(ParserTest_SyntaxError_Test, TestBody) {
    EXPECT_EQ(parse("123"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
}


