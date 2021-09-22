package krypto.ui.orders;

import krypto.serialization.Side;
import krypto.ui.components.CenterAlignedTableCellRenderer;
import krypto.ui.components.ColorConstants;

import javax.swing.*;
import java.awt.*;

public class FillCellRenderer extends CenterAlignedTableCellRenderer {
  @Override
  public Component getTableCellRendererComponent(
      JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
    final Component c =
        super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
    final String sideStr = table.getValueAt(row, FillsTable.Column.SIDE.ordinal()).toString();
    if (sideStr.equals(Side.name(Side.BUY))) {
      c.setBackground(ColorConstants.BID_BACKGROUND_COLOR);
      return c;
    }

    if (sideStr.equals(Side.name(Side.SELL))) {
      c.setBackground(ColorConstants.ASK_BACKGROUND_COLOR);
      return c;
    }

    return c;
  }
}
