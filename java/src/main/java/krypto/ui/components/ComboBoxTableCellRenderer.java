package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.util.function.Function;

public class ComboBoxTableCellRenderer<T> extends DefaultTableCellRenderer {

    private final Function<T, String> getter;

    public ComboBoxTableCellRenderer(final Function<T, String> getter) {
        this.getter = getter;
    }

    public Component getTableCellRendererComponent(
            JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        if (value != null) {
            setText(getter.apply((T) value));
        }

        if (isSelected) {
            setBackground(table.getSelectionBackground());
        } else {
            setBackground(table.getSelectionForeground());
        }

        return this;
    }
}
