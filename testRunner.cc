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

TEST(GridTest, ImportStateFromFile) {
  // Create a Grid object
  Grid grid;

  // Open the test file
  std::ifstream testFile("inputLife.txt");
  ASSERT_TRUE(testFile.is_open()) << "Failed to open test file";

  // Import the state from the file
  grid.importStateFromFile(testFile);

  // Read the expected state from another file
  std::ifstream expectedFile("outputLife.txt");
  ASSERT_TRUE(expectedFile.is_open()) << "Failed to open expected file";

  // Create a Grid object to store the expected state
  Grid expectedGrid;

  // Import the expected state from the file
  expectedGrid.importStateFromFile(expectedFile);

  // Compare the imported state with the expected state
  for (int row = 0; row < grid.getNumRows(); ++row) {
    for (int col = 0; col < grid.getNumCols(); ++col) {
      EXPECT_EQ(grid.getCell(row, col).isAlive(),
                expectedGrid.getCell(row, col).isAlive());
    }
  }
}

// TEST(GridStressTest, LargeGrid) {
//   const int numRows = 100;
//   const int numCols = 100;
//   const int numGenerations = 1000;

//   Grid grid(numRows, numCols);

//   // Set initial state with random values
//   grid.setRandomInitialState(0.3);

//   // Simulate multiple generations
//   for (int generation = 1; generation <= numGenerations; ++generation) {
//     grid.calcNextGeneration();
//   }

//   // Ensure that the grid has the expected dimensions
//   EXPECT_EQ(grid.getNumRows(), numRows);
//   EXPECT_EQ(grid.getNumCols(), numCols);

//   // Print the final generation for inspection
//   //   grid.printLastGeneration(numGenerations - 1);
// }

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}