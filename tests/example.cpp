#include <gtest/gtest.h>

namespace {
int GetMeaningOfLife() { return 42; }
} // namespace

// Expect continues the test whiel assert stop the test instantly, so if you
// want the test to continue use expect

TEST(TestTopic, TrivialEquality) { EXPECT_EQ(GetMeaningOfLife(), 42); }

TEST(TestTopic, MoreEqualityTests) {
  ASSERT_EQ(GetMeaningOfLife(), 0) << "Oh no, a mistake!";
  EXPECT_FLOAT_EQ(23.23F, 23.23F);
}
