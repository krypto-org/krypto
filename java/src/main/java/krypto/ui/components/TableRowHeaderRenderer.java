package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.JTableHeader;
import java.awt.*;
import java.math.BigDecimal;
import java.math.RoundingMode;

public class TableRowHeaderRenderer extends JLabel implements ListCellRenderer<Double> {
  public TableRowHeaderRenderer(JTable table) {
    JTableHeader header = table.getTableHeader();
    setOpaque(true);
    setBorder(UIManager.getBorder("TableHeader.cellBorder"));
    setHorizontalAlignment(CENTER);
    setForeground(header.getForeground());
    setBackground(header.getBackground());
    setFont(header.getFont());
  }

  @Override
  public Component getListCellRendererComponent(
      JList<? extends Double> list,
      Double value,
      int index,
      boolean isSelected,
      boolean cellHasFocus) {
    setFont(FontConstants.TABLE_HEADER_FONT);
    setText(
        (value == null) ? "" : new BigDecimal(value).setScale(0, RoundingMode.CEILING).toString());
    setHorizontalAlignment(SwingConstants.CENTER);
    setVerticalAlignment(SwingConstants.CENTER);
    return this;
  }
}
