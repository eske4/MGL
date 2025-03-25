#include <gtest/gtest.h>

#include "parser.tab.h"
#include "lexer.h"
#include "error_handler.h"


class UnitTestToken : public testing::Test {
 protected:
  void SetUp() override {
      setTestMode(true);
  }
};

TEST_F(UnitTestToken, Keyword) {
    EXPECT_EQ(tokenize("Map"), TK_MAP) << "Map wasn't recognized as a keyword";
    EXPECT_EQ(tokenize("Room"), TK_ROOM) << "Room wasn't recognized as a keyword";
    EXPECT_EQ(tokenize("Connect"), TK_CONNECT) << "Connect wasn't recognized as a keyword";
}

TEST_F(UnitTestToken, IntegerLiteral) {
    EXPECT_EQ(tokenize("123"), TK_INTEGER) << "123 wasn't recognized as an integer literal";
    EXPECT_EQ(tokenize("4567"), TK_INTEGER) << "4567 wasn't recognized as an integer literal";
}

TEST_F(UnitTestToken, FloatLiteral) {
    EXPECT_EQ(tokenize("123.45"), TK_FLOAT) << "123.45 wasn't recognized as a float literal";
    EXPECT_EQ(tokenize("0.99"), TK_FLOAT) << "0.99 wasn't recognized as a float literal";
}

TEST_F(UnitTestToken, StringLiteral) {
    EXPECT_EQ(tokenize("\"Hello World\""), TK_STRING) << "\"Hello World\" wasn't recognized as a string literal";
    EXPECT_EQ(tokenize("\"This is a test\""), TK_STRING) << "\"This is a test\" wasn't recognized as a string literal";
    EXPECT_NE(tokenize("\n"), TK_STRING) << "\"This is a test\" wasn't recognized as a string literal";
    EXPECT_NE(tokenize("test"), TK_STRING) << "\"This is a test\" wasn't recognized as a string literal";
}

TEST_F(UnitTestToken, Identifier) {
    EXPECT_EQ(tokenize("myVariable"), TK_IDENTIFIER) << "myVariable wasn't recognized as an identifier";
    EXPECT_EQ(tokenize("variable_123"), TK_IDENTIFIER) << "variable_123 wasn't recognized as an identifier";

    EXPECT_NE(tokenize("123variable"), TK_IDENTIFIER) << "123variable should be recognized as an invalid identifier";
}

TEST_F(UnitTestToken, OrdOperator) {
    EXPECT_EQ(tokenize("<="), TK_ORD_OPERATOR) << "<= wasn't recognized as an operator";
    EXPECT_EQ(tokenize(">="), TK_ORD_OPERATOR) << ">= wasn't recognized as an operator";
    EXPECT_EQ(tokenize("<"), TK_ORD_OPERATOR) << "< wasn't recognized as an operator";
    EXPECT_EQ(tokenize(">"), TK_ORD_OPERATOR) << "> wasn't recognized as an operator";
    EXPECT_EQ(tokenize("AND"), TK_ORD_OPERATOR) << "AND wasn't recognized as an operator";
    EXPECT_EQ(tokenize("OR"), TK_ORD_OPERATOR) << "OR wasn't recognized as an operator";
}

TEST_F(UnitTestToken, EqOperator) {
    EXPECT_EQ(tokenize("=="), TK_EQ_OPERATOR) << "<= wasn't recognized as an operator";
    EXPECT_EQ(tokenize("!="), TK_EQ_OPERATOR) << "<= wasn't recognized as an operator";
}

TEST_F(UnitTestToken, GraphOperators) {
    EXPECT_EQ(tokenize("->"), TK_DIRECTED_EDGE) << "-> wasn't recognized as an operator";
    EXPECT_EQ(tokenize("<->"), TK_BIDIRECTIONAL_EDGE) << "<-> wasn't recognized as an operator";
    EXPECT_NE(tokenize("-->"), TK_DIRECTED_EDGE) << "-> wasn't recognized as an operator";
}

// Testing for special characters that are tokens but not normally part of valid syntax
TEST_F(UnitTestToken, Braces) {
    EXPECT_EQ(tokenize("{"), TK_LEFT_BRACE) << "{ wasn't recognized as a left brace";
    EXPECT_EQ(tokenize("}"), TK_RIGHT_BRACE) << "} wasn't recognized as a right brace";
}

TEST_F(UnitTestToken, Paranthesis) {
    EXPECT_EQ(tokenize("("), TK_LEFT_PAREN) << "{ wasn't recognized as a left brace";
    EXPECT_EQ(tokenize(")"), TK_RIGHT_PAREN) << "{ wasn't recognized as a left brace";
}

TEST_F(UnitTestToken, Semicolon) {
    EXPECT_EQ(tokenize(";"), TK_SEMICOLON) << "; wasn't recognized as a semicolon";
}

TEST_F(UnitTestToken, Comment) {
    EXPECT_EQ(tokenize("# This is a comment"), 0) << "# This is a comment should be ignored as a comment";
}
