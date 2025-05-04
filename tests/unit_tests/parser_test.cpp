#include <gtest/gtest.h>
extern "C"{
    #include "parser.h"
    }

// A helper to hold fake token stream
static std::vector<Token> __fakeTokens;
static size_t         __fakeIndex;

// Stub to override the real `scan()` when we link.
#ifdef PARSER_UNIT_TEST
extern "C" int scan(Token* out) {
  if (__fakeIndex < __fakeTokens.size()) {
    *out = __fakeTokens[__fakeIndex++];
    return out->token;
  }
  out->token    = T_EOF;
  out->value[0] = '\0';
  return T_EOF;
}
#endif

TEST(ParserUnitTest, HappyPathFullMap) {
    // --- Arrange ----------------------------------------------------
  
    __fakeTokens = {
      { T_MAP,                0, "Map"   },
      { T_IDENTIFIER,         0, "map"   },
      { T_LBRACE,             0, "{"     },
  
      { T_ROOM,               0, "Room"  },
      { T_IDENTIFIER,         0, "A"     },
      { T_SEMICOLON,          0, ";"     },
  
      { T_ROOM,               0, "Room"  },
      { T_IDENTIFIER,         0, "B"     },
      { T_SEMICOLON,          0, ";"     },
  
      { T_CONNECT,            0, "Connect"      },
      { T_LPAREN,             0, "("            },
      { T_IDENTIFIER,         0, "A"            },
      { T_DIRECTED_EDGE,      0, "->"           },
      { T_IDENTIFIER,         0, "B"            },
      { T_RPAREN,             0, ")"            },
      { T_SEMICOLON,          0, ";"            },
  
      { T_ROOM,               0, "Room"  },
      { T_IDENTIFIER,         0, "E"     },
      { T_SEMICOLON,          0, ";"     },
  
      { T_ROOM,               0, "Room"  },
      { T_IDENTIFIER,         0, "D"     },
      { T_SEMICOLON,          0, ";"     },
  
      { T_CONNECT,            0, "Connect"      },
      { T_LPAREN,             0, "("            },
      { T_IDENTIFIER,         0, "E"            },
      { T_BIDIRECTIONAL_EDGE, 0, "<->"          },
      { T_IDENTIFIER,         0, "D"            },
      { T_RPAREN,             0, ")"            },
      { T_SEMICOLON,          0, ";"            },
  
      { T_RBRACE,             0, "}"            },
      { T_EOF,                0, ""             }
    };
    __fakeIndex = 0;
    Token cur = __fakeTokens[__fakeIndex++];  // prime first token
  
    // --- Act --------------------------------------------------------
    ASTree tree = parse(&cur);
  
    // --- Assert -----------------------------------------------------
    ASSERT_NE(tree, nullptr);
  
    ASTNode* root = tree->head;
    EXPECT_EQ(root->type, AT_MAP);
    EXPECT_EQ(root->child_count, 7);
  
    // 0: map name
    EXPECT_EQ(root->children[0]->type, AT_IDENTIFIER);
    EXPECT_STREQ(root->children[0]->data, "map");
  
    // 1: Room A
    EXPECT_EQ(root->children[1]->type, AT_ROOM);
    EXPECT_STREQ(root->children[1]->children[0]->data, "A");
  
    // 2: Room B
    EXPECT_EQ(root->children[2]->type, AT_ROOM);
    EXPECT_STREQ(root->children[2]->children[0]->data, "B");
  
    // 3: Connect(A->B)
    {
      ASTNode* c = root->children[3];
      EXPECT_EQ(c->type, AT_CONNECT);
      EXPECT_EQ(c->child_count, 3);
      EXPECT_STREQ(c->children[0]->data, "A");
      EXPECT_EQ(c->children[1]->type, AT_DIRECTED_EDGE);
      EXPECT_STREQ(c->children[2]->data, "B");
    }
  
    // 4: Room E
    EXPECT_EQ(root->children[4]->type, AT_ROOM);
    EXPECT_STREQ(root->children[4]->children[0]->data, "E");
  
    // 5: Room D
    EXPECT_EQ(root->children[5]->type, AT_ROOM);
    EXPECT_STREQ(root->children[5]->children[0]->data, "D");
  
    // 6: Connect(E<->D)
    {
      ASTNode* c = root->children[6];
      EXPECT_EQ(c->type, AT_CONNECT);
      EXPECT_EQ(c->child_count, 3);
      EXPECT_STREQ(c->children[0]->data, "E");
      EXPECT_EQ(c->children[1]->type, AT_BIDIRECTIONAL_EDGE);
      EXPECT_STREQ(c->children[2]->data, "D");
    }
  
    ASTFree(tree);
}
  
TEST(ParserUnitTest, UnexpectedTokenInMapBody) {
    // --- Arrange ----------------------------------------------------

    // Build tokens:  Map map { map }
    __fakeTokens = {
        { T_MAP,        0, "Map"   },
        { T_IDENTIFIER, 0, "map"   },
        { T_LBRACE,     0, "{"     },
        { T_IDENTIFIER, 0, "map"   },   // <-- unexpected here
        { T_RBRACE,     0, "}"     },
        { T_EOF,        0, ""      }
    };
    __fakeIndex = 0;
    Token cur = __fakeTokens[__fakeIndex++];  // prime the first token
  
    // --- Act & Assert ---------------------------------------
    EXPECT_EXIT(
      parse(&cur),
      ::testing::ExitedWithCode(1),
      ".*"
    );
}
  