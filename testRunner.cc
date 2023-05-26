#include <gtest/gtest.h>

#include "grid.h"

TEST(GridTest, Initialization) {
  Grid grid(10, 10);
  // Check that all cells are initially dead
  for (int row = 0; row < 10; ++row) {
    for (int col = 0; col < 10; ++col) {
      EXPECT_EQ(grid.getCell(row, col).isAlive(), false);
    }
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}