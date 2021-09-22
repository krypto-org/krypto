package krypto.ui.components;

import java.util.ArrayList;

public class CellSelectionSet {
  private final ArrayList<Cell> cells = new ArrayList<>();

  public void add(int r, int c) {
    if (!contains(r, c)) {
      cells.add(new Cell(r, c));
    }
  }

  public boolean containsOneOrLess() {
    return cells.size() <= 1;
  }

  public boolean contains(int r, int c) {
    for (Cell cell : cells) {
      if (cell.is(r, c)) {
        return true;
      }
    }
    return false;
  }

  public void clear() {
    cells.clear();
  }

  public ArrayList<Cell> getCells() {
    return cells;
  }
}
