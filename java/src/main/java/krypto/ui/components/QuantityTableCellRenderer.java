package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;

public class QuantityTableCellRenderer extends DefaultTableCellRenderer {
  private final Color foregroundColor;
  private final Color backgroundColorPositive;
  private final Color backgroundColorNegative;
  private final Color elseBackgroundColor;
  private final int column;
  private final boolean alignCenter;

  public QuantityTableCellRenderer(
      Color foregroundColor,
      Color elseBackgroundColor,
      Color foregroundColorPositive,
      Color foregroundColorNegative,
      int column,
      boolean alignCenter) {
    super();
    this.foregroundColor = foregroundColor;
    this.backgroundColorPositive = foregroundColorPositive;
    this.backgroundColorNegative = foregroundColorNegative;
    this.elseBackgroundColor = elseBackgroundColor;
    this.column = column;
    this.alignCenter = alignCenter;
  }

  @Override
  public Component getTableCellRendererComponent(
      JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
    Component renderer =
        super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
    if (column == this.column) {
      if (!isSelected) {
        renderer.setBackground(this.elseBackgroundColor);
      }

      if (value != null) {
        if (Double.parseDouble(value.toString()) > 0) {
          renderer.setForeground(this.foregroundColor);
          renderer.setBackground(this.backgroundColorPositive);
        }

        if (Double.parseDouble(value.toString()) < 0) {
          renderer.setForeground(this.foregroundColor);
          renderer.setBackground(this.backgroundColorNegative);
        }

        if (this.alignCenter) {
          ((DefaultTableCellRenderer) renderer).setHorizontalAlignment(SwingConstants.CENTER);
        }
      }
    }
    return renderer;
  }
}
