#include <gtest/gtest.h>
#include "parser.h"

#define PARSER_SUCCEEDED 0
#define PARSER_FAILED 1
#define MEMORY_FAILED 2

// Testing valid tokens like keywords and literals


TEST(ParserTest, Connect) {
    EXPECT_EQ(parse("Connect(Entrance -> Hallway);"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Connect(Entrance Hallway);"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
}


