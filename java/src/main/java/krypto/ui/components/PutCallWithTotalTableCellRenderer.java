package krypto.ui.components;

import javax.swing.*;
import java.awt.*;

public class PutCallWithTotalTableCellRenderer extends PutCallTableCellRenderer {
    public PutCallWithTotalTableCellRenderer(Color putBackgroundColor,
                                             Color callBackgroundColor, Color foregroundColor, int column,
                                             boolean alignCenter) {
        super(putBackgroundColor, callBackgroundColor, foregroundColor, column,
                alignCenter);
    }

    @Override
    public Component getTableCellRendererComponent(JTable table, Object value,
                                                   boolean isSelected, boolean hasFocus, int row, int column) {
        Component renderer = super.getTableCellRendererComponent(table, value, isSelected, hasFocus,
                row, column);
        if (row == table.getRowCount() - 1) {
            renderer.setBackground(ColorConstants.TOTAL_COLOR);
        }
        return renderer;
    }
}
