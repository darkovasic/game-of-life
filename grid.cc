#include "grid.h"

using std::cin;
using std::cout;
using std::endl;
using std::stoi;
using std::string;

Grid::Grid(int numRows, int numCols)
    : m_numRows(numRows), m_numCols(numCols), m_cells(numRows * numCols) {}

int Grid::getNumRows() const { return m_numRows; }

int Grid::getNumCols() const { return m_numCols; }

void Grid::setNumRows(int NumRows) { m_numRows = NumRows; }

void Grid::setNumCols(int numCols) { m_numCols = numCols; }

void Grid::setCells() { m_cells.resize(m_numRows * m_numCols); }

const Cell& Grid::getCell(int row, int col) const {
  int index = row * m_numCols + col;
  return m_cells[index];
}

bool charToBool(char const& ch) {
  string str(1, ch);
  return str != "0";
}

void Grid::setRandomInitialState(double probability) {
  std::random_device rand;
  std::mt19937 gen(rand());
  std::uniform_real_distribution<> dis(0.0, 1.0);

  for (int row = 0; row < m_numRows; row++) {
    for (int col = 0; col < m_numCols; col++) {
      double randomValue = dis(gen);
      bool alive = (randomValue < probability);
      setCell(row, col, alive);
    }
  }
}

std::vector<int> Grid::setInitialState(std::vector<string>& lines) {
  std::vector<int> gridSettings;

  m_numRows = stoi(lines.at(0));
  m_numCols = stoi(lines.at(1));
  m_cells.resize(m_numRows * m_numCols);
  gridSettings.push_back(stoi(lines.at(2)));
  gridSettings.push_back(stoi(lines.at(3)));

  lines.erase(lines.begin(), lines.begin() + 4);

  for (int row = 0; row < m_numRows; row++) {
    for (int col = 0; col < m_numCols; col++) {
      bool alive = charToBool(lines[row][col]);
      setCell(row, col, alive);
    }
  }

  return gridSettings;
}

std::vector<int> Grid::importStateFromFile(std::ifstream& file) {
  string line;
  std::vector<string> words;

  int lineNumber = 1;
  while (std::getline(file, line)) {
    if (line.empty()) continue;
    std::istringstream iss(line);
    string word;
    if (lineNumber == 1 || lineNumber == 2) {
      // Split the string by spaces
      while (iss >> word) {
        words.push_back(word);
      }
    } else {
      if (line == "EOF") break;
      words.push_back(line);
    }
    lineNumber++;
  }
  file.close();

  std::vector<int> gridSettings = setInitialState(words);

  return gridSettings;
}

void Grid::writeFile(int gen) {
  std::filesystem::path filename;
  cout << "Please enter filename: ";
  cin >> filename;

  if (std::filesystem::exists(filename)) {
    std::cout << "\nFile already exists.\n";
    cout << "Please enter filename: ";
    cin >> filename;
  }

  std::ofstream file(filename);

  if (file.is_open()) {
    file << m_numRows << " " << m_numCols << endl;
    file << gen << " " << gen << endl;
    for (int i = 0; i < m_numRows; i++) {
      for (int j = 0; j < m_numCols; j++) {
        const Cell& cell = getCell(i, j);
        file << (cell.isAlive() ? "1" : "0");
      }
      file << endl;
    }
    file << "\nEOF\n\n";
    file.close();
    cout << "File created successfully.\n";
  } else {
    cout << "Failed to create the file.\n";
  }
}

void Grid::setCell(int row, int col, bool alive) {
  int index = row * m_numCols + col;
  m_cells[index].setAlive(alive);
}

Cell& Grid::getCell(int row, int col) {
  int index = row * m_numCols + col;
  return m_cells[index];
}

int Grid::countAliveNeighbors(int row, int col) const {
  int count = 0;

  // Check the 8 neighboring cells
  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      // Skip the current cell
      if (i == 0 && j == 0) continue;

      // Calculate the coordinates of the neighboring cell
      int neighborRow = row + i;
      int neighborCol = col + j;

      // Check if the neighboring cell is within the grid boundaries
      if (neighborRow >= 0 && neighborRow < m_numRows && neighborCol >= 0 &&
          neighborCol < m_numCols) {
        // Check if the neighboring cell is alive
        if (m_cells[neighborRow * m_numCols + neighborCol].isAlive()) count++;
      }
    }
  }

  return count;
}

void wait(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Grid::updateAndSaveGenerations(int numGenerations, int delay) {
  m_generations.clear();             // Clear any existing generations
  m_generations.push_back(m_cells);  // Save the initial state

  // Output the initial state of the grid
  // cout << "\nInitial state:\n";
  // printGeneration(0);

  for (int i = 0; i < numGenerations; i++) {
    std::vector<Cell> newCells(m_cells.size());

    for (int row = 0; row < m_numRows; row++) {
      for (int col = 0; col < m_numCols; col++) {
        int index = row * m_numCols + col;
        const Cell& cell = m_cells[index];
        int numNeighbors = countAliveNeighbors(row, col);
        newCells[index] = cell.getNextGeneration(numNeighbors);
      }
    }

    m_generations.push_back(newCells);  // Save the new generation
    wait(delay);
    m_cells = std::move(newCells);  // Update the current generation
    printGeneration(i);             // Print the new generation
  }
}

void Grid::printGeneration(GenerationIndex generation) const {
  if (generation < m_generations.size()) {
    cout << "Generation " << generation + 1 << ":\n";
    const std::vector<Cell>& cells = m_generations[generation];

    for (int row = 0; row < m_numRows; row++) {
      for (int col = 0; col < m_numCols; col++) {
        int index = row * m_numCols + col;
        const Cell& cell = cells[index];
        std::cout << (cell.isAlive() ? "X" : " ") << " ";
      }
      std::cout << "\n";
    }
  }
}

void Grid::previousGeneration() {
  if (m_generations.size() >= 2) {
    cout << "Generation size: " << m_generations.size() << endl;
    m_generations.pop_back();
    int lastIndex = m_generations.size() - 2;
    cout << "Generation size: " << m_generations.size() << endl;
    printGeneration(lastIndex);
  } else {
    cout << "\nThere is only one generation.\n\n";
  }
}