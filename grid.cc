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
    std::cerr << "\nFile already exists.\n";
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
    std::cerr << "Failed to create the file.\n";
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

  for (int i = 0; i < numGenerations; i++) {
    calcNextGeneration();
    wait(delay);
  }
}

void Grid::calcNextGeneration() {
  int index;
  std::vector<Cell> newCells(m_cells.size());

  for (int row = 0; row < m_numRows; row++) {
    for (int col = 0; col < m_numCols; col++) {
      index = row * m_numCols + col;
      const Cell& cell = m_cells[index];
      int numNeighbors = countAliveNeighbors(row, col);
      newCells[index] = cell.getNextGeneration(numNeighbors);
    }
  }

  m_generations.push_back(newCells);         // Save the new generation
  m_cells = std::move(newCells);      // Update the current generation
  int lastIndex = m_generations.size() - 2;  // Get index of the last iterator
  printLastGeneration(lastIndex);            // Print the new generation
}

void Grid::printLastGeneration(GenerationIndex generation) const {
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
  if (m_generations.size() > 1) {
    m_generations.pop_back();
    int lastIndex = m_generations.size() - 2;
    printLastGeneration(lastIndex);
  } else {
    std::cerr << "\nThere is only one generation.\n\n";
  }
}

void Grid::nextGeneration() {
  if (m_generations.size() > 20) {
    m_generations.pop_front();
  }
  calcNextGeneration();
}

void Grid::gridToImage(const std::string& outputFilename) {
  const int width = m_numCols * 10;   // Image width
  const int height = m_numRows * 10;  // Image height
  const std::vector<Cell>* lastGeneration = nullptr;

  std::vector<uint8_t> image(width * height * 4,
                             255);  // RGBA format with white background

  if (m_generations.size() >= 2) {
    lastGeneration = &m_generations[m_generations.size() - 2];
  } else {
    std::cerr << "Not enough generations in m_generations" << std::endl;
  }

  // Update image based on grid state
  if (lastGeneration) {
    for (int row = 0; row < m_numRows; ++row) {
      for (int col = 0; col < m_numCols; ++col) {
        const Cell& cell = (*lastGeneration)[row * m_numCols + col];
        const bool cellState = cell.isAlive();
        const int startX = col * 10;
        const int startY = (m_numRows - row - 1) * 10;  // Reverse the row index

        // Set cell color based on state
        const uint8_t color = cellState ? 0 : 255;

        // Update image pixels for the cell
        for (int x = startX; x < startX + 10; ++x) {
          for (int y = startY; y < startY + 10; ++y) {
            const int pixelIndex = (y * width + x) * 4;
            image[pixelIndex] = color;      // Red channel
            image[pixelIndex + 1] = color;  // Green channel
            image[pixelIndex + 2] = color;  // Blue channel
            image[pixelIndex + 3] = 255;    // Alpha channel
          }
        }
      }
    }

    // Reverse the image rows
    std::vector<uint8_t> reversedImage(width * height * 4);
    for (int row = 0; row < height; ++row) {
      const int reversedRow = height - row - 1;
      std::memcpy(&reversedImage[reversedRow * width * 4],
                  &image[row * width * 4], width * 4);
    }

    // Open output file for writing using FILE*
    FILE* file = fopen(outputFilename.c_str(), "wb");
    if (!file) {
      std::cerr << "Error opening output file" << std::endl;
      return;
    }

    // Initialize libpng structures
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                              nullptr, nullptr);
    if (!png) {
      std::cerr << "Error creating png_struct" << std::endl;
      fclose(file);
      return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
      std::cerr << "Error creating png_info" << std::endl;
      png_destroy_write_struct(&png, nullptr);
      fclose(file);
      return;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png))) {
      std::cerr << "Error writing PNG image" << std::endl;
      png_destroy_write_struct(&png, &info);
      fclose(file);
      return;
    }

    // Set output stream
    png_init_io(png, file);

    // Set image attributes
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    // Write image data
    std::vector<png_bytep> rowPointers(height);
    for (int y = 0; y < height; ++y) {
      rowPointers[y] =
          reinterpret_cast<png_bytep>(&reversedImage[y * width * 4]);
    }
    png_set_rows(png, info, rowPointers.data());
    png_write_png(png, info, PNG_TRANSFORM_IDENTITY, nullptr);

    // Cleanup
    png_destroy_write_struct(&png, &info);
    fclose(file);

    std::cout << "Image saved to " << outputFilename << std::endl;
  } else {
    std::cerr << "No last generation available" << std::endl;
  }
}
