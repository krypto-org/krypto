package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;

public class CenterAlignedTableCellRenderer extends DefaultTableCellRenderer {
    @Override
    public Component getTableCellRendererComponent(JTable table, Object value,
                                                   boolean isSelected, boolean hasFocus, int row, int column) {

        Component renderer = super.getTableCellRendererComponent(table, value,
                isSelected, hasFocus, row, column);

        ((DefaultTableCellRenderer) renderer)
                .setHorizontalAlignment(SwingConstants.CENTER);

        return renderer;
    }
}
