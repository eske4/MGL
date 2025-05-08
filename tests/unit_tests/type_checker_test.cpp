#include <gtest/gtest.h>
extern "C" {
  #include "astree.h"
  #include "semantic_analyzer.h"
}

TEST(TypeCheckerUnitTest, HappyPathFullMap) {
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
    int rc = TypeCheck(tree);
  
    // --- Assert -----------------------------------------------------
    EXPECT_EQ(1, rc) << "TypeCheck should succeed on a well‐formed map";
  
    // clean up
    ASTFree(tree);
}

TEST(TypeCheckerUnitTest, DuplicateRoom) {
    // --- Arrange ----------------------------------------------------
    // Map M { Room A; Room A; }
    ASTree tree = ASTInit();
    ASSERT_NE(tree, nullptr);

    ASTNode* root = ASTCreateMap(tree, "map", 0);
    tree->head = root;
  
    ASTCreateRoom(root, "A", 10);
    ASTCreateRoom(root, "A", 20);
  
    // --- Act & Assert -----------------------------------------------
    EXPECT_EXIT(
      TypeCheck(tree),
      ::testing::ExitedWithCode(1),
      ".*"
    );
  
    ASTFree(tree);
}

TEST(TypeCheckerUnitTest, RoomNameClashesWithMap) {
    // --- Arrange ----------------------------------------------------

    // Map M { Room M; }
    ASTree tree = ASTInit();
    ASSERT_NE(tree, nullptr);
    ASTNode* root = ASTCreateMap(tree, "M", 0);
    tree->head = root;
  
    ASTCreateRoom(root, "M", 10);
  
    // --- Act & Assert -----------------------------------------------
    EXPECT_EXIT(
      TypeCheck(tree),
      ::testing::ExitedWithCode(1),
      ".*"
    );
  
    ASTFree(tree);
}

TEST(TypeCheckerUnitTest, DuplicateConnection) {
    // --- Arrange ----------------------------------------------------

    // Map M { Room A; Room B; Connect(A->B); Connect(A->B); }
    ASTree tree = ASTInit();
    ASSERT_NE(tree, nullptr);
    ASTNode* root = ASTCreateMap(tree, "map", 0);
    tree->head = root;

    ASTCreateRoom(root, "A", 1);
    ASTCreateRoom(root, "B", 2);
    ASTCreateConnect(root, "A", AT_DIRECTED_EDGE, "B", 3);
    ASTCreateConnect(root, "A", AT_DIRECTED_EDGE, "B", 4);

    // --- Act & Assert -----------------------------------------------
    EXPECT_EXIT(
        TypeCheck(tree),
        ::testing::ExitedWithCode(1),
        ".*"
    );

    ASTFree(tree);
}

TEST(TypeCheckerUnitTest, UndeclaredRoomInConnect) {
    // --- Arrange ----------------------------------------------------

    // Map M { Connect(A->B); }
    ASTree tree = ASTInit();
    ASSERT_NE(tree, nullptr);
    ASTNode* root = ASTCreateMap(tree, "map", 0);
    tree->head = root;
  
    ASTCreateConnect(root, "A", AT_DIRECTED_EDGE, "B", 5);
  
    // --- Act & Assert -----------------------------------------------
    EXPECT_EXIT(
      TypeCheck(tree),
      ::testing::ExitedWithCode(1),
      ".*"
    );
  
    ASTFree(tree);
}

TEST(TypeCheckerUnitTest, ConnectionEndpointWrongType) {
    // --- Arrange ----------------------------------------------------

    // Map M { Room A; Connect(A -> M); }
    ASTree tree = ASTInit();
    ASSERT_NE(tree, nullptr);
    ASTNode* root = ASTCreateMap(tree, "M", 0);
    tree->head = root;
  
    ASTCreateRoom(root, "A", 1);
    ASTCreateConnect(root, "A", AT_DIRECTED_EDGE, "M", 2);
  
    // --- Act & Assert -----------------------------------------------
    EXPECT_EXIT(
      TypeCheck(tree),
      ::testing::ExitedWithCode(1),
      ".*"
    );
  
    ASTFree(tree);
}
   

TEST(TypeCheckerUnitTest, SelfConnection) {
    // --- Arrange ----------------------------------------------------

    // Map M { Room A; Connect(A<->A); }
    ASTree tree = ASTInit();
    ASSERT_NE(tree, nullptr);
    ASTNode* root = ASTCreateMap(tree, "map", 0);
    tree->head = root;

    ASTCreateRoom(root, "A", 1);
    ASTCreateConnect(root, "A", AT_BIDIRECTIONAL_EDGE, "A", 2);

    // --- Act & Assert -----------------------------------------------
    EXPECT_EXIT(
        TypeCheck(tree),
        ::testing::ExitedWithCode(1),
        ".*"
    );

    ASTFree(tree);
}
