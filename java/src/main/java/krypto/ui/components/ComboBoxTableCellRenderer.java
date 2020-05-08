package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;
import java.awt.*;
import java.util.function.Function;

public class ComboBoxTableCellRenderer<T> extends JComboBox<T> implements TableCellRenderer {
    public ComboBoxTableCellRenderer(final T[] items) {
        super(items);
    }

    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected,
                                                   boolean hasFocus, int row, int column) {
        if (isSelected) {
            setForeground(table.getSelectionForeground());
            super.setBackground(table.getSelectionBackground());
        } else {
            setForeground(table.getForeground());
            setBackground(table.getBackground());
        }
        setSelectedItem(value);
        return this;
    }
}
