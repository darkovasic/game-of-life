#include <chrono>
#include <iostream>
#include <thread>

using std::cin;
using std::cout;
using std::endl;

#include "grid.h"

void wait(int ms) {
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

int main(int argc, char* argv[]) {
  int numRows;
  int numColumns;
  int numGenerations;
  int delay;

  // Initialize empty grid
  Grid grid(0, 0);

  if (argc == 2) {
    std::ifstream file(argv[1]);
    if (file.is_open()) {
      std::vector<int> gridSettings = grid.importStateFromFile(file);
      numGenerations = gridSettings.at(0);
      delay = gridSettings.at(1);
    }
  } else if (argc > 2) {
    cout << "Please provide exactly one argument.\n";
  } else {
    cout << "Enter width of the playing field: ";
    cin >> numRows;
    cout << "Enter height of the playing field: ";
    cin >> numColumns;
    cout << "Enter number of generations: ";
    cin >> numGenerations;
    cout << "Enter delay between generations in milliseconds: ";
    cin >> delay;

    grid.setNumRows(numRows);
    grid.setNumCols(numColumns);
    grid.setCells();
    grid.setRandomInitialState(0.3);
  }

  // Output the initial state of the grid
  cout << endl << "Initial state:" << endl;
  grid.print();

  // Simulate specified number of generations
  for (int gen = 1; gen <= numGenerations; gen++) {
    grid.update();
    wait(delay);

    // Output the state of the grid after the update
    cout << "Generation " << gen << ":\n";
    grid.print();
  }

  int selectedOption;
  cout << "1. Go back one generation\n";
  cout << "2. Go forward one generation\n";
  cout << "3. Save current state to a file\n";
  cout << "4. Quit\n\n";
  cout << "Choose an option [default: 4]: ";
  cin >> selectedOption;

  switch (selectedOption) {
    case 3:
      grid.writeFile(numGenerations);
      break;

    default:
      return 0;
  }
}
