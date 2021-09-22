package krypto.ui.components;

import javax.swing.*;
import java.awt.*;
import java.util.EventObject;

public class QuadTableCellEditor extends DefaultCellEditor {
  private QuadTableCellPanel panel;

  public QuadTableCellEditor(JCheckBox checkBox) {
    super(checkBox);
    this.panel = new QuadTableCellPanel();
    this.panel.setOpaque(true);
  }

  @Override
  public Component getTableCellEditorComponent(
      JTable table, Object value, boolean isSelected, int row, int column) {
    panel.setBackground(isSelected ? table.getSelectionBackground() : table.getBackground());
    return panel;
  }

  @Override
  public boolean isCellEditable(EventObject anEvent) {
    return false;
  }
}
