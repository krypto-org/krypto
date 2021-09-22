package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableModel;

public class LiveEmptyRowFilter extends RowFilter<TableModel, Object> {

  @Override
  public boolean include(Entry<? extends TableModel, ? extends Object> entry) {
    for (int i = 0; i < entry.getValueCount(); i++) {
      String value = entry.getStringValue(i);
      if (value.toLowerCase().indexOf("") != -1) return true;
    }

    return false;
  }
}
