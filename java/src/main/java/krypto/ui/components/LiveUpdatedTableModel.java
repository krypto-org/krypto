package krypto.ui.components;

import java.util.Vector;

public class LiveUpdatedTableModel extends ReadOnlyTableModel {
  public LiveUpdatedTableModel() {
    super();
  }

  @SuppressWarnings({"unchecked", "rawtypes"})
  @Override
  public void setValueAt(Object value, int row, int column) {
    ((Vector) dataVector.get(row)).set(column, value);
  }

  public void updateTable() {
    super.fireTableRowsUpdated(0, this.getRowCount() - 1);
  }
}
