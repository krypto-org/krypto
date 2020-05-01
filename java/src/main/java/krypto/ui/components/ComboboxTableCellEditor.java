package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableModel;
import java.awt.*;
import java.util.List;

public class ComboboxTableCellEditor<T> extends AbstractCellEditor implements
        TableCellEditor {

    private final JComboBox<T> comboBox;

    @SuppressWarnings("unchecked")
    public ComboboxTableCellEditor(List<T> values) {
        this.comboBox = new JComboBox<T>((T[]) values.toArray());
    }

    @Override
    public Object getCellEditorValue() {
        return comboBox.getSelectedItem();
    }

    @Override
    public Component getTableCellEditorComponent(JTable table, Object value,
                                                 boolean isSelected, int row, int column) {
        if (isSelected) {
            comboBox.setSelectedItem(value);
            TableModel model = table.getModel();
            model.setValueAt(value, row, column);
        }

        return comboBox;
    }

}
