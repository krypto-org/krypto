package krypto.ui.orders;

import krypto.ui.components.CenterAlignedTableCellRenderer;
import krypto.ui.components.ColorConstants;

import javax.swing.*;
import java.awt.*;

public class ActiveOrderTableCellRenderer extends CenterAlignedTableCellRenderer {
    @Override
    public Component getTableCellRendererComponent(
            JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        final Component c =
                super.getTableCellRendererComponent(
                        table, value, isSelected, hasFocus, row, column);
        final String status = table.getValueAt(row, OrdersTable.Column.STATUS.ordinal()).toString();

        if (!(status.equals("DONE") || status.equals("CANCELLED"))) {
            c.setBackground(ColorConstants.ACTIVE_ORDER_COLOR);
            c.setForeground(Color.BLACK);
        } else {
            c.setBackground(ColorConstants.TOTAL_COLOR);
            c.setForeground(Color.WHITE);
        }

        return c;
    }
}
