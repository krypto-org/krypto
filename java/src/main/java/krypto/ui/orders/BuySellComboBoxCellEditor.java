package krypto.ui.orders;

import krypto.ui.components.ColorConstants;
import krypto.ui.components.ComboBoxTableCellEditor;

import javax.swing.*;
import java.awt.*;

public class BuySellComboBoxCellEditor extends ComboBoxTableCellEditor<Side> {
  public BuySellComboBoxCellEditor(final Side[] values) {
    super(values);
  }

  @Override
  public Component getTableCellEditorComponent(
      JTable table, Object value, boolean isSelected, int row, int column) {
    final Component c = super.getTableCellEditorComponent(table, value, isSelected, row, column);
    if (value.toString().equals(Side.BUY.toString())) {
      c.setBackground(ColorConstants.BID_BACKGROUND_COLOR);
      c.setForeground(Color.WHITE);
      return c;
    }

    if (value.toString().equals(Side.SELL.toString())) {
      c.setBackground(ColorConstants.ASK_BACKGROUND_COLOR);
      c.setForeground(Color.WHITE);
      return c;
    }
    return c;
  }
}
