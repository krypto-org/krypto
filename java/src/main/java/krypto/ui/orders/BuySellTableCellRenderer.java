package krypto.ui.orders;

import krypto.ui.components.CenterAlignedTableCellRenderer;
import krypto.ui.components.ColorConstants;

import javax.swing.*;
import java.awt.*;

public class BuySellTableCellRenderer extends CenterAlignedTableCellRenderer {

    @Override
    public Component getTableCellRendererComponent(
            JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        final Component c =
                super.getTableCellRendererComponent(
                        table, value, isSelected, hasFocus, row, column);
        if (value.toString().equals(Side.BUY.toString())) {
            c.setBackground(ColorConstants.BID_BACKGROUND_COLOR);
            c.setForeground(Color.WHITE);
            return c;
        }

        if (value.toString().equals(Side.SELL.toString())) {
            c.setBackground(ColorConstants.ASK_BACKGROUND_COLOR);
            c.setForeground(Color.WHITE);
            return c;
        }
        return c;
    }
}
