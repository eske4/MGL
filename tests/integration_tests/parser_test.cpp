#include <gtest/gtest.h>
#include "parser.h"
#include "error_handler.h"

#define PARSER_SUCCEEDED 0
#define PARSER_FAILED 1
#define MEMORY_FAILED 2

class ParserIntegrationTest : public testing::Test {
 protected:
  void SetUp() override {
      setTestMode(true);
  }

};



TEST_F(ParserIntegrationTest, MapDecl) {
    // Valid map declarations
    EXPECT_EQ(parse("Map map { Connect(Entrance -> Hallway); }"), PARSER_SUCCEEDED);
    EXPECT_EQ(parse("Map map { Room name; }"), PARSER_SUCCEEDED);
    EXPECT_EQ(parse("Map map {  Room name; Connect(Entrance -> Hallway); Room name2;}"), PARSER_SUCCEEDED);
    
    // Extra spaces
    EXPECT_EQ(parse("Map map {Connect(Entrance -> Hallway); }"), PARSER_SUCCEEDED);  // Missing spaces
    EXPECT_EQ(parse("Map  map { Connect(Entrance -> Hallway); }"), PARSER_SUCCEEDED); // Extra space should be allowed

    // Invalid map declarations (syntax errors)
    EXPECT_EQ(parse("Map map; { Room name; }"), PARSER_FAILED); // Missing braces
    EXPECT_EQ(parse("Map Connect(Entrance --> Hallway;"), PARSER_FAILED); // Syntax error in connect
    EXPECT_EQ(parse("Map Room name;"), PARSER_FAILED); // Invalid syntax: room declared outside map

    // Typo in the map keyword
    EXPECT_EQ(parse("map map { Connect(Entrance -> Hallway); }"), PARSER_FAILED); // Typo in "Map"
    EXPECT_EQ(parse("Mpa map { Connect(Entrance -> Hallway); }"), PARSER_FAILED); // Typo in "Map"

    // Missing or extra spaces
}

TEST_F(ParserIntegrationTest, MapImp) {
    // Valid imperative map declaration with rooms and connections
    EXPECT_EQ(parse("Map map; Room name; Room name2; Connect(name1 -> name2);"), PARSER_SUCCEEDED);

    // Invalid imperative map declarations
    EXPECT_EQ(parse("Map; Room name; Room name2; Connect(name1 -> name2);"), PARSER_FAILED); // Missing map name
    EXPECT_EQ(parse("Room; Room name; Room name2; Connect(name1 -> name2);"), PARSER_FAILED); // Invalid declaration
    EXPECT_EQ(parse("room; Room name; Room name2; Connect(name1 -> name2);"), PARSER_FAILED); // 'room' instead of 'Room'
    EXPECT_EQ(parse("Room name; Room name2; Connect(name1 -> name2);"), PARSER_FAILED); // Missing 'Map' keyword at start

    // Typo in imperative syntax
    EXPECT_EQ(parse("Map map; room name; Room name2; Connect(name1 -> name2);"), PARSER_FAILED); // 'room' instead of 'Room'
    EXPECT_EQ(parse("Map map Room name; Room name2; Connect(name1 -> name2);"), PARSER_FAILED); // Missing semicolon after Map
    EXPECT_EQ(parse("Map map; Room name; Connect(name1 -> name2"), PARSER_FAILED);  // Missing closing parenthesis in Connect
}

TEST_F(ParserIntegrationTest, Connect) {
    // Valid connect declarations
    EXPECT_EQ(parse("Map map; Connect(Entrance -> Hallway);"), PARSER_SUCCEEDED);
    EXPECT_EQ(parse("Map map; Connect(Room1 -> Room2);"), PARSER_SUCCEEDED);
    EXPECT_EQ(parse("Map map;Connect(Entrance -> Hallway);"), PARSER_SUCCEEDED); // Missing space after 'Map map;'

    // Invalid connect declarations (syntax errors)
    EXPECT_EQ(parse("Map map; Connect(Entrance Hallway);"), PARSER_FAILED); // Missing '->'
    EXPECT_EQ(parse("Connect(Entrance Hallway);"), PARSER_FAILED); // Missing 'Map'
    EXPECT_EQ(parse("Map map; ConnectEntrance -> Hallway);"), PARSER_FAILED); // Typo in 'Connect'
    EXPECT_EQ(parse("Map map; Connect(Entrance -> Hallway;"), PARSER_FAILED); // Missing closing parenthesis
    EXPECT_EQ(parse("Map; Connect(Entrance -> Hallway;"), PARSER_FAILED); // Missing map declaration
    EXPECT_EQ(parse("Map map Connect(Entrance -> Hallway;"), PARSER_FAILED); // Missing semicolon after map declaration

    // Missing or extra spaces in connect
    EXPECT_EQ(parse("Map map ; Connect(Entrance -> Hallway);"), PARSER_SUCCEEDED);  // Extra space should be allowed
    EXPECT_EQ(parse("Map map; Connect ( Entrance -> Hallway );"), PARSER_SUCCEEDED);  // Extra spaces should be allowed

    // Typo in connect parameters
    EXPECT_EQ(parse("Map map; Connect(Entrance --> Hallway);"), PARSER_FAILED); // Invalid arrow symbol '-->' instead of '->'
    EXPECT_EQ(parse("Map map; Connect(Entrance -> Hallway;)"), PARSER_FAILED);  // Extra semicolon
}

TEST_F(ParserIntegrationTest, Room) {
    // Valid room declaration
    EXPECT_EQ(parse("Map map; Room name;"), PARSER_SUCCEEDED);
    EXPECT_EQ(parse("Map map; Room entrance; Room hallway;"), PARSER_SUCCEEDED);

    // Invalid room declarations
    EXPECT_EQ(parse("Map map; room name;"), PARSER_FAILED); // Typo: 'room' instead of 'Room'
    EXPECT_EQ(parse("Map map; Room;"), PARSER_FAILED);  // Missing room name
    EXPECT_EQ(parse("Map map; room name"), PARSER_FAILED); // Missing semicolon after room declaration
    EXPECT_EQ(parse("Map map; name"), PARSER_FAILED); // Missing 'Room' keyword
    EXPECT_EQ(parse("Map map; Room name; Room ;"), PARSER_FAILED); // Missing room name after 'Room'

    // Typos in room keyword
    EXPECT_EQ(parse("Map map; rroom name;"), PARSER_FAILED); // Typo in 'Room'
    EXPECT_EQ(parse("Map map; Room ;"), PARSER_FAILED);  // Missing room name
}
