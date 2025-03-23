#include <gtest/gtest.h>
#include "parser.h"

#define PARSER_SUCCEEDED 0
#define PARSER_FAILED 1
#define MEMORY_FAILED 2


TEST(ParserIntegrationTest, MapDecl) {
    EXPECT_EQ(parse("Map map { Connect(Entrance -> Hallway); }"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_EQ(parse("Map map { Room name; }"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_EQ(parse("Map map; Room name; Room name2; Connect(name1 -> name2);"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map { Room name; Room name2; Connect(name1 -> name2);"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map Room name; Room name2; Connect(name1 -> name2);"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map; { Room name; }"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map Connect(Entrance --> Hallway;"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map Room name;"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
}


TEST(ParserIntegrationTest, Connect) {
    EXPECT_EQ(parse("Map map; Connect(Entrance -> Hallway);"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map; Connect(Entrance Hallway);"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Connect(Entrance Hallway);"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map; ConnectEntrance --> Hallway);"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map; Connect(Entrance --> Hallway;"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map; Connect(Entrance --> Hallway;"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map Connect(Entrance --> Hallway;"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map Connect(Entrance --> Hallway;"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
}

TEST(ParserIntegrationTest, Room) {
    EXPECT_EQ(parse("Map map; Room name;"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map; room name;"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map; Room;"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map; room name"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
    EXPECT_NE(parse("Map map; name"), PARSER_SUCCEEDED);  // 1 typically indicates a syntax error in Bison
}




