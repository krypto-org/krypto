package krypto.ui.components;

import javax.swing.table.DefaultTableModel;

public class ParameterValueTableModel extends DefaultTableModel {
  public ParameterValueTableModel() {
    super();
    this.addColumn("PARAMETER");
    this.addColumn("VALUE");
  }

  @Override
  public boolean isCellEditable(int row, int column) {
    boolean rv = true;

    if (column == 0) {
      rv = false;
    }

    return rv;
  }
}
