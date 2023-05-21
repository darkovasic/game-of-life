#pragma once

class Cell {
 public:
  Cell(bool alive = false);
  bool isAlive() const;
  void setAlive(bool alive);
  Cell getNextGeneration(int numNeighbors) const;

 private:
  bool m_alive;
};
