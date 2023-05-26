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

TEST(GridTest, SetInitialState) {
  Grid grid(5, 5);

  // Set specific cells as alive
  grid.setCell(2, 2, true);
  grid.setCell(3, 3, true);

  // Check the state of the cells
  EXPECT_EQ(grid.getCell(2, 2).isAlive(), true);
  EXPECT_EQ(grid.getCell(3, 3).isAlive(), true);

  // Check that other cells are still dead
  for (int row = 0; row < 5; ++row) {
    for (int col = 0; col < 5; ++col) {
      if (row != 2 && col != 2 && row != 3 && col != 3) {
        EXPECT_EQ(grid.getCell(row, col).isAlive(), false);
      }
    }
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}