#include <gtest/gtest.h>
#include "file_config.c"

extern "C"{
#include "compiler_state.h"
}

int compareCLoc(const CLoc* a, const CLoc* b) {
    return (a->line == b->line) && (a->column == b->column);
}

class UnitTestErrorLocator : public testing::Test {
    protected:
        void SetUp() override {
            csInit();
            csOpenFile(SOURCE_CODE_FILE);
        }
};

TEST_F(UnitTestErrorLocator, FindLocation) {
    CLoc expected = {2,12};
    CLoc result = findLoc(21);
    EXPECT_TRUE(compareCLoc(&result, &expected));

    CLoc expected2 = {5,21};
    CLoc result2 = findLoc(54);
    EXPECT_TRUE(compareCLoc(&result2, &expected2));
}

