package krypto.ui.components;

import javax.swing.*;

public class ComboBoxTableCellEditor<T> extends DefaultCellEditor {
  public ComboBoxTableCellEditor(final T[] values) {
    super(new JComboBox<>(values));
  }
}
