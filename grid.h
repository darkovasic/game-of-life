#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "cell.h"

class Grid {
 public:
  Grid(int numRows, int numCols);
  int getNumRows() const;
  int getNumCols() const;
  void setNumRows(int numRows);
  void setNumCols(int numCols);
  void setCells();
  const Cell& getCell(int row, int col) const;
  std::vector<int> setInitialState(std::vector<std::string>&);
  void setRandomInitialState(double probability);
  void print();
  std::vector<int> importStateFromFile(std::ifstream& file);
  int countAliveNeighbors(int row, int col) const;
  void writeFile(int gen);
  void setCell(int row, int col, bool alive);
  Cell& getCell(int row, int col);
  void update();

 private:
  int m_numRows;
  int m_numCols;
  std::vector<Cell> m_cells;
};
