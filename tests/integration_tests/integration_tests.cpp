#include <gtest/gtest.h>
#include "file_config.c"
extern "C" {
    #include "compiler_state.h"
    #include "lexer.h"
    #include "definitions.h"
    #include "error_handler.h"
    #include "parser.h"
    #include "semantic_analyzer.h"
    #include "il.h"
}

class IntegrationTestErrorLocator : public testing::Test {
protected:
    void SetUp() override {
        csInit();
        setTestMode(true);
        csOpenFile(INTEGRATION_TEST_FILE);
    }
};

TEST_F(IntegrationTestErrorLocator, Lexer_Test) {
    Token tok;
    int result = scan(&tok);
    
    ASSERT_EQ(result, T_MAP);               // The scan() function returns token type
    EXPECT_EQ(tok.token, T_MAP);            // Confirm token type stored in struct
    EXPECT_STREQ(tok.value, "Map");         // Confirm token value/lexeme
}


TEST_F(IntegrationTestErrorLocator, Parser_Test) {
    Token tok;
    scan(&tok); // Initialize the lexer
    ASTree tree = parse(&tok); // Parse the input

    ASSERT_NE(tree, nullptr);     // Ensure parsing produced a tree
    ASSERT_NE(tree->head, nullptr); // Check that the tree isn't empty
}

TEST_F(IntegrationTestErrorLocator, TypeCheck_Test) {
    Token tok;
    scan(&tok); // Lexer
    ASTree tree = parse(&tok); // Parser

    ASSERT_NE(tree, nullptr);
    int result = TypeCheck(tree); // Run type checker

    EXPECT_EQ(result, 1); // Expect successful type check
    ASTFree(tree);
}

TEST_F(IntegrationTestErrorLocator, Intermediate_Test) {
    Token tok;
    scan(&tok); // Lexer
    ASTree tree = parse(&tok); // Parser

    ASSERT_NE(tree, nullptr);
    int result = TypeCheck(tree); // Type checker
    ASSERT_EQ(result, 1);

    InstructionTable table = compile_to_il(tree); // Intermediate code generation
    ASSERT_NE(table, nullptr); // Check that IL generation succeeded

    instr_free(table);
    ASTFree(tree);
}
