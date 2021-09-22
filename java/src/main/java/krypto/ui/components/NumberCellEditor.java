package krypto.ui.components;

import javax.swing.*;
import java.awt.*;
import java.text.NumberFormat;

public class NumberCellEditor extends DefaultCellEditor {

  private final NumberFormat formatter;
  private final JTextField textField;

  public NumberCellEditor(NumberFormat formatter) {
    super(new JTextField());
    this.formatter = formatter;
    this.textField = (JTextField) this.getComponent();
    textField.setHorizontalAlignment(JTextField.RIGHT);
    textField.setBorder(null);
  }

  @Override
  public Object getCellEditorValue() {
    try {
      return Double.parseDouble(textField.getText());
    } catch (final NumberFormatException e) {
      return (double) 0;
    }
  }

  @Override
  public Component getTableCellEditorComponent(
      JTable table, Object value, boolean isSelected, int row, int column) {
    textField.setText((value == null) ? "" : formatter.format((Double) value));
    return textField;
  }
}
