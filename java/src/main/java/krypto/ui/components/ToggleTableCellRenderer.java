package krypto.ui.components;

import javax.swing.*;
import java.awt.*;

public class ToggleTableCellRenderer extends CenterAlignedTableCellRenderer {
    private JToggleButton toggleButton = new JToggleButton();

    private final int column;
    private final String onLabel;
    private final String offLabel;

    public ToggleTableCellRenderer(final int column, String onLabel, String offLabel) {
        this.column = column;
        this.onLabel = onLabel;
        this.offLabel = offLabel;
    }

    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        if (column == this.column) {
            boolean active = (Boolean) value;
            if (active) {
                toggleButton.setText(onLabel);
            } else {
                toggleButton.setText(offLabel);
            }
            toggleButton.setSelected(active);
            return toggleButton;
        } else {
            return super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
        }
    }
}