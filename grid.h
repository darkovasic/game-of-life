#pragma once

#include <algorithm>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "cell.h"

using std::string;
using std::vector;
using GenerationIndex = typename std::deque<std::vector<Cell>>::size_type;

class Grid {
 public:
  Grid(int numRows, int numCols);
  int getNumRows() const;
  int getNumCols() const;
  void setNumRows(int numRows);
  void setNumCols(int numCols);
  void setCells();
  const Cell& getCell(int row, int col) const;
  vector<int> setInitialState(vector<string>&);
  void setRandomInitialState(double probability);
  void printGeneration(GenerationIndex generation) const;
  vector<int> importStateFromFile(std::ifstream& file);
  int countAliveNeighbors(int row, int col) const;
  void writeFile(int gen);
  void setCell(int row, int col, bool alive);
  Cell& getCell(int row, int col);
  void updateAndSaveGenerations(int numGenerations, int delay);
  void previousGeneration();
  void nextGeneration();
  void calcNextGeneration();

 private:
  int m_numRows;
  int m_numCols;
  vector<Cell> m_cells;
  std::deque<vector<Cell>> m_generations;
};
