package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableCellEditor;
import javax.swing.table.TableModel;
import java.awt.*;

public class SpinnerTableCellEditor extends AbstractCellEditor implements
        TableCellEditor {
    private final JSpinner spinner;

    public SpinnerTableCellEditor(SpinnerNumberModel model) {
        spinner = new JSpinner(model);
    }

    @Override
    public Component getTableCellEditorComponent(JTable table, Object value,
                                                 boolean isSelected, int row, int column) {

        if (isSelected) {
            spinner.setValue(value);
            TableModel model = table.getModel();
            model.setValueAt(value, row, column);
        }

        return spinner;
    }

    @Override
    public Object getCellEditorValue() {
        return spinner.getValue();
    }
}