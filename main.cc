#include "grid.h"

using std::cin;
using std::cout;
using std::endl;

int showGameOptions() {
  int selectedOption;
  cout << "1. Go back one generation\n";
  cout << "2. Go forward one generation\n";
  cout << "3. Save current state to a file\n";
  cout << "4. Save currently shown field to PNG file\n";
  cout << "5. Quit\n\n";
  cout << "Choose an option [default: 5]: ";
  cin >> selectedOption;

  return selectedOption;
}

int main(int argc, char* argv[]) {
  int numRows;
  int numColumns;
  int numGenerations;
  int delay;

  // Initialize empty grid
  Grid grid;

  if (argc == 2) {
    std::ifstream file(argv[1]);
    if (file.is_open()) {
      std::vector<int> gridSettings = grid.importStateFromFile(file);
      numGenerations = gridSettings.at(0);
      delay = gridSettings.at(1);
    }
  } else if (argc > 2) {
    std::cerr << "Please provide exactly one argument.\n";
    return 0;
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

  grid.updateAndSaveGenerations(numGenerations, delay);

  while (true) {
    int selectedOption = showGameOptions();

    switch (selectedOption) {
      case 1:
        grid.previousGeneration();
        break;

      case 2:
        grid.nextGeneration();
        break;

      case 3:
        grid.writeFile(numGenerations);
        break;

      case 4:
        grid.gridToImage("grid.png");
        break;

      default:
        return 0;
    }
  }
}
