#include <gtest/gtest.h>
extern "C" {
  #include "astree.h"
  #include "instr_table.h"
  #include "il.h"
}

TEST(ILUnitTest, HappyPathFullMap) {
    // --- Arrange ----------------------------------------------------
    
    // Build AST for:
    //   Map map {
    //     Room A;
    //     Room B;
    //     Connect(A -> B);
    //     Room E;
    //     Room D;
    //     Connect(E <-> D);
    //   }

    ASTree tree = ASTInit();
    ASSERT_NE(tree, nullptr);
    ASTNode* root = ASTCreateMap(tree, "map", 0);
    tree->head = root;
    ASTCreateRoom(root, "A", 1);
    ASTCreateRoom(root, "B", 2);
    ASTCreateConnect(root, "A", AT_DIRECTED_EDGE, "B", 3);
    ASTCreateRoom(root, "E", 4);
    ASTCreateRoom(root, "D", 5);
    ASTCreateConnect(root, "E", AT_BIDIRECTIONAL_EDGE, "D", 6);
  
    // --- Act --------------------------------------------------------
    InstructionTable il = compile_to_il(tree);
  
    // --- Assert -----------------------------------------------------
    // 1 map, 4 rooms, 1 directed, 2 for bidirectional = 8 instructions
    ASSERT_EQ(il->count, 8);
  
    // 0: Map
    EXPECT_EQ(il->entries[0].InstrCode, IR_DECL_MAP);
    EXPECT_EQ(il->entries[0].argc, 1);
    EXPECT_STREQ(il->entries[0].args[0], "map");
  
    // 1: Room A
    EXPECT_EQ(il->entries[1].InstrCode, IR_DECL_ROOM);
    EXPECT_EQ(il->entries[1].argc, 1);
    EXPECT_STREQ(il->entries[1].args[0], "A");
  
    // 2: Room B
    EXPECT_EQ(il->entries[2].InstrCode, IR_DECL_ROOM);
    EXPECT_EQ(il->entries[2].argc, 1);
    EXPECT_STREQ(il->entries[2].args[0], "B");
  
    // 3: Connect A->B
    EXPECT_EQ(il->entries[3].InstrCode, IR_DECL_CONNECT);
    ASSERT_EQ(il->entries[3].argc, 2);
    EXPECT_STREQ(il->entries[3].args[0], "A");
    EXPECT_STREQ(il->entries[3].args[1], "B");
  
    // 4: Room E
    EXPECT_EQ(il->entries[4].InstrCode, IR_DECL_ROOM);
    EXPECT_EQ(il->entries[4].argc, 1);
    EXPECT_STREQ(il->entries[4].args[0], "E");
  
    // 5: Room D
    EXPECT_EQ(il->entries[5].InstrCode, IR_DECL_ROOM);
    EXPECT_EQ(il->entries[5].argc, 1);
    EXPECT_STREQ(il->entries[5].args[0], "D");
  
    // 6: Connect E->D (forward)
    EXPECT_EQ(il->entries[6].InstrCode, IR_DECL_CONNECT);
    ASSERT_EQ(il->entries[6].argc, 2);
    EXPECT_STREQ(il->entries[6].args[0], "E");
    EXPECT_STREQ(il->entries[6].args[1], "D");
  
    // 7: Connect D->E (reverse)
    EXPECT_EQ(il->entries[7].InstrCode, IR_DECL_CONNECT);
    ASSERT_EQ(il->entries[7].argc, 2);
    EXPECT_STREQ(il->entries[7].args[0], "D");
    EXPECT_STREQ(il->entries[7].args[1], "E");
  
    // clean up
    instr_free(il);
    ASTFree(tree);
}