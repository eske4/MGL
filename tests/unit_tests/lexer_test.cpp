#include <gtest/gtest.h>
#include "lexer.h"
#include "token_def.h"

// Testing valid tokens like keywords and literals

TEST(UnitTestToken, Keyword) {
    EXPECT_EQ(tokenize("Map"), TK_MAP) << "Map wasn't recognized as a keyword";
    EXPECT_EQ(tokenize("Room"), TK_ROOM) << "Room wasn't recognized as a keyword";
    EXPECT_EQ(tokenize("Connect"), TK_CONNECT) << "Connect wasn't recognized as a keyword";
}

TEST(UnitTestToken, IntegerLiteral) {
    EXPECT_EQ(tokenize("123"), TK_INTEGER) << "123 wasn't recognized as an integer literal";
    EXPECT_EQ(tokenize("4567"), TK_INTEGER) << "4567 wasn't recognized as an integer literal";

    EXPECT_EQ(tokenize("01"), TK_INVALID_INTEGER) << "123abc should be recognized as an invalid integer";
    EXPECT_EQ(tokenize("001"), TK_INVALID_INTEGER) << "++123 should be recognized as an invalid integer";
}

TEST(UnitTestToken, FloatLiteral) {
    EXPECT_EQ(tokenize("123.45"), TK_FLOAT) << "123.45 wasn't recognized as a float literal";
    EXPECT_EQ(tokenize("0.99"), TK_FLOAT) << "0.99 wasn't recognized as a float literal";

    EXPECT_EQ(tokenize("123.45.67"), TK_INVALID_FLOAT) << "123.45.67 should be recognized as an invalid float";
    EXPECT_EQ(tokenize("123..45"), TK_INVALID_FLOAT) << "123..45 should be recognized as an invalid float";
    EXPECT_EQ(tokenize("0.99.99"), TK_INVALID_FLOAT) << "0.99.99 should be recognized as an invalid float";
}

TEST(UnitTestToken, StringLiteral) {
    EXPECT_EQ(tokenize("\"Hello World\""), TK_STRING) << "\"Hello World\" wasn't recognized as a string literal";
    EXPECT_EQ(tokenize("\"This is a test\""), TK_STRING) << "\"This is a test\" wasn't recognized as a string literal";

    EXPECT_EQ(tokenize("\"UnclosedString"), TK_INVALID_STRING) << "\"UnclosedString should be recognized as an invalid string";
    EXPECT_EQ(tokenize("\"Invalid\\string"), TK_INVALID_STRING) << "\"Invalid\\string should be recognized as an invalid string";
}

TEST(UnitTestToken, Identifier) {
    EXPECT_EQ(tokenize("myVariable"), TK_IDENTIFIER) << "myVariable wasn't recognized as an identifier";
    EXPECT_EQ(tokenize("variable_123"), TK_IDENTIFIER) << "variable_123 wasn't recognized as an identifier";

    EXPECT_EQ(tokenize("123variable"), TK_INVALID_IDENTIFIER) << "123variable should be recognized as an invalid identifier";
}

TEST(UnitTestToken, OrdOperator) {
    EXPECT_EQ(tokenize("=="), TK_ORD_OPERATOR) << "== wasn't recognized as an operator";
    EXPECT_EQ(tokenize("<="), TK_ORD_OPERATOR) << "<= wasn't recognized as an operator";
    EXPECT_EQ(tokenize(">="), TK_ORD_OPERATOR) << ">= wasn't recognized as an operator";
    EXPECT_EQ(tokenize("<"), TK_ORD_OPERATOR) << "< wasn't recognized as an operator";
    EXPECT_EQ(tokenize(">"), TK_ORD_OPERATOR) << "> wasn't recognized as an operator";
    EXPECT_EQ(tokenize("AND"), TK_ORD_OPERATOR) << "AND wasn't recognized as an operator";
    EXPECT_EQ(tokenize("OR"), TK_ORD_OPERATOR) << "OR wasn't recognized as an operator";
}

TEST(UnitTestToken, GraphOperators) {
    EXPECT_EQ(tokenize("->"), TK_DIRECTED_EDGE) << "-> wasn't recognized as an operator";
    EXPECT_EQ(tokenize("<->"), TK_BIDIRECTIONAL_EDGE) << "<-> wasn't recognized as an operator";
}

// Testing for special characters that are tokens but not normally part of valid syntax
TEST(UnitTestToken, Braces) {
    EXPECT_EQ(tokenize("{"), TK_LEFT_BRACE) << "{ wasn't recognized as a left brace";
    EXPECT_EQ(tokenize("}"), TK_RIGHT_BRACE) << "} wasn't recognized as a right brace";
}

TEST(UnitTestToken, Paranthesis) {
    EXPECT_EQ(tokenize("("), TK_LEFT_PAREN) << "{ wasn't recognized as a left brace";
    EXPECT_EQ(tokenize(")"), TK_RIGHT_PAREN) << "{ wasn't recognized as a left brace";
}

TEST(UnitTestToken, Semicolon) {
    EXPECT_EQ(tokenize(";"), TK_SEMICOLON) << "; wasn't recognized as a semicolon";
}

TEST(UnitTestToken, Comment) {
    EXPECT_EQ(tokenize("# This is a comment"), 0) << "# This is a comment should be ignored as a comment";
}


// Testing invalid tokens

TEST(UnitTestToken, InvalidSyntax) {
    EXPECT_EQ(tokenize(""), 0) << "Empty string should be recognized as invalid syntax";
}

