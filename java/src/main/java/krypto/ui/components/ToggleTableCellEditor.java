package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableModel;
import java.awt.*;
import java.util.function.Function;

public class ToggleTableCellEditor<T> extends AbstractCellEditor
    implements TableCellEditor, TableCellRenderer {

  private final JToggleButton toggleButton = new JToggleButton();

  private final String activeLabel;
  private final String inactiveLabel;

  private final Function<String, T> valueGetter;

  public ToggleTableCellEditor(
      String activeLabel, String inactiveLabel, final Function<String, T> valueGetter) {
    this.activeLabel = activeLabel;
    this.inactiveLabel = inactiveLabel;
    this.valueGetter = valueGetter;
  }

  @Override
  public Object getCellEditorValue() {
    return toggleButton.isSelected() ? activeLabel : inactiveLabel;
  }

  @Override
  public Component getTableCellEditorComponent(
      JTable table, Object value, boolean isSelected, int row, int column) {
    if (isSelected && value != null) {
      TableModel model = table.getModel();
      model.setValueAt(this.valueGetter.apply(value.toString()), row, column);
    }
    return toggleButton;
  }

  @Override
  public Component getTableCellRendererComponent(
      JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
    boolean active = (Boolean) value;
    if (active) {
      toggleButton.setText(activeLabel);
    } else {
      toggleButton.setText(inactiveLabel);
    }
    toggleButton.setSelected(active);
    return toggleButton;
  }
}
