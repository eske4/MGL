#include <gtest/gtest.h>
#include "file_config.c"
extern "C"{
    #include "compiler_state.h"
    #include "lexer.h"
    }

TEST(LexerUnitTest, HappyPathFullMap) {
    // --- Arrange --------------------------------------------------
    csInit();
    std::string path = std::string(TEST_FILE_DIR) + "input_success.MF";
    ASSERT_EQ(1, csOpenFile(path.c_str()))
      << "couldn't open " << path;
  
    Token t;
  
    struct Expected { TokenDef token; const char *lexeme; };
    std::vector<Expected> expect = {
      { T_MAP,                 "Map" },
      { T_IDENTIFIER,          "map" },
      { T_LBRACE,              "{"   },
      { T_ROOM,                "Room" },
      { T_IDENTIFIER,          "A"   },
      { T_SEMICOLON,           ";"   },
      { T_ROOM,                "Room" },
      { T_IDENTIFIER,          "B"   },
      { T_SEMICOLON,           ";"   },
      { T_CONNECT,             "Connect" },
      { T_LPAREN,              "("   },
      { T_IDENTIFIER,          "A"   },
      { T_DIRECTED_EDGE,       "->"  },
      { T_IDENTIFIER,          "B"   },
      { T_RPAREN,              ")"   },
      { T_SEMICOLON,           ";"   },
      { T_ROOM,                "Room" },
      { T_IDENTIFIER,          "E"   },
      { T_SEMICOLON,           ";"   },
      { T_ROOM,                "Room" },
      { T_IDENTIFIER,          "D"   },
      { T_SEMICOLON,           ";"   },
      { T_CONNECT,             "Connect" },
      { T_LPAREN,              "("   },
      { T_IDENTIFIER,          "E"   },
      { T_BIDIRECTIONAL_EDGE,  "<->" },
      { T_IDENTIFIER,          "D"   },
      { T_RPAREN,              ")"   },
      { T_SEMICOLON,           ";"   },
      { T_RBRACE,              "}"   },
      { T_EOF,                 ""    }
    };
  
    // --- Act & Assert ----------------------------------------------
    for (size_t i = 0; i < expect.size(); ++i) {
      int tok = scan(&t);
      // token kind
      EXPECT_EQ(expect[i].token, tok)
        << "at #" << i << ", expected token " << expect[i].token
        << " but got " << tok;
      // lexeme text (for those that carry one)
      if (expect[i].lexeme && *expect[i].lexeme) {
        EXPECT_STREQ(expect[i].lexeme, t.value)
          << "at #" << i << ", lexeme";
      }
    }
}

TEST(LexerUnitTest, LexicalError) {
    // --- Arrange --------------------------------------------------
    setTestMode(1);

    csInit();
    std::string path = std::string(TEST_FILE_DIR) + "input_fail.MF";
    ASSERT_EQ(1, csOpenFile(path.c_str()))
      << "couldn't open " << path;
  
    Token t;
  
    int rc = scan(&t);
    EXPECT_EQ(ERR_LEXER, rc)
      << "Expected ERR_LEXER when the file starts with '@', got " << rc;
}