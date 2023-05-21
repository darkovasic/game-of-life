#include "cell.h"

Cell::Cell(bool alive) : m_alive(alive = false) {}

bool Cell::isAlive() const { return m_alive; }

void Cell::setAlive(bool alive) { m_alive = alive; }

Cell Cell::getNextGeneration(int numNeighbors) const {
  Cell nextCell;
  if (m_alive) {
    if (numNeighbors < 2 || numNeighbors > 3) {
      nextCell.setAlive(
          false);  // Cell dies due to underpopulation or overpopulation
    } else {
      nextCell.setAlive(true);  // Cell survives to the next generation
    }
  } else {
    if (numNeighbors == 3) {
      nextCell.setAlive(true);  // Cell becomes alive due to reproduction
    } else {
      nextCell.setAlive(false);  // Cell remains dead in the next generation
    }
  }
  return nextCell;
}