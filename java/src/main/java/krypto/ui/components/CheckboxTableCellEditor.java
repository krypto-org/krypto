package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableModel;
import java.awt.*;

public class CheckboxTableCellEditor extends AbstractCellEditor implements TableCellEditor {
    private final JCheckBox checkbox;

    public CheckboxTableCellEditor() {
        checkbox = new JCheckBox();
    }

    @Override
    public Object getCellEditorValue() {
        String rv = "NO";
        if (checkbox.isSelected()) {
            rv = "YES";
        }
        return rv;
    }

    @Override
    public Component getTableCellEditorComponent(JTable table, Object value,
                                                 boolean isSelected, int row, int column) {
        if (isSelected) {
            checkbox.setSelected((boolean) value);
            TableModel model = table.getModel();
            model.setValueAt(value, row, column);
        }

        return checkbox;
    }

}
