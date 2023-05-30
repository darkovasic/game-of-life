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

TEST(GridTest, NextGeneration) {
  // Create a grid with a specific initial state
  Grid grid(4, 4);
  grid.setCell(1, 1, true);
  grid.setCell(1, 2, true);
  grid.setCell(2, 1, true);
  grid.setCell(2, 2, true);

  // Get the next generation
  grid.calcNextGeneration();

  // Define the expected states for each cell
  std::vector<std::pair<int, int>> cellsToCheck = {
      {1, 1}, {1, 2}, {2, 1}, {2, 2}, {0, 0}, {0, 1}, {0, 2},
      {1, 0}, {2, 0}, {3, 0}, {3, 1}, {3, 2}, {3, 3}};

  // Check the state of each cell in the next generation
  for (const auto& cell : cellsToCheck) {
    int row = cell.first;
    int col = cell.second;
    bool expectedAlive =
        (row >= 1 && row <= 2) &&
        (col >= 1 && col <= 2);  // Cells in the middle survive, others die
    EXPECT_EQ(grid.getCell(row, col).isAlive(), expectedAlive);
  }
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}