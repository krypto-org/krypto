package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableModel;
import java.awt.*;
import java.util.List;
import java.util.function.Function;

public class ComboBoxTableCellEditor<T> extends AbstractCellEditor implements TableCellEditor {

    private final JComboBox<T> comboBox;
    private final Function<T, String> getter;

    public ComboBoxTableCellEditor(final List<T> values) {
        this(values, Object::toString);
    }

    public ComboBoxTableCellEditor(final List<T> values, final Function<T, String> getter) {
        this.comboBox = new JComboBox<>();
        values.forEach(this.comboBox::addItem);
        this.getter = getter;
    }

    @Override
    public Object getCellEditorValue() {
        return this.getter.apply(comboBox.getItemAt(comboBox.getSelectedIndex()));
    }

    @Override
    public Component getTableCellEditorComponent(
            JTable table, Object value, boolean isSelected, int row, int column) {
        if (isSelected && value != null) {
            comboBox.setSelectedItem(value);
            TableModel model = table.getModel();
            model.setValueAt(value, row, column);
        }
        return comboBox;
    }
}
