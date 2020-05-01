package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;

public class PutCallTableCellRenderer extends DefaultTableCellRenderer {

    private final Color putBackgroundColor;
    private final Color callBackgroundColor;

    private final Color foregroundColor;
    private final int column;
    private final boolean alignCenter;

    public PutCallTableCellRenderer(Color putBackgroundColor,
                                    Color callBackgroundColor, Color foregroundColor, int column,
                                    boolean alignCenter) {
        super();
        this.putBackgroundColor = putBackgroundColor;
        this.callBackgroundColor = callBackgroundColor;
        this.foregroundColor = foregroundColor;
        this.column = column;
        this.alignCenter = alignCenter;
    }

    @Override
    public Component getTableCellRendererComponent(JTable table, Object value,
                                                   boolean isSelected, boolean hasFocus, int row, int column) {
        Component renderer = super.getTableCellRendererComponent(table, value,
                isSelected, hasFocus, row, column);
        if (column == this.column) {
            if (!isSelected) {
                if (value.toString().contains(" C")) {
                    renderer.setBackground(this.callBackgroundColor);
                } else if (value.toString().contains(" P")) {
                    renderer.setBackground(this.putBackgroundColor);
                }

                if (renderer.getBackground() == Color.BLACK) {
                    renderer.setForeground(this.foregroundColor);
                }
            }

            if (this.alignCenter) {
                ((DefaultTableCellRenderer) renderer)
                        .setHorizontalAlignment(SwingConstants.CENTER);
            }

        }
        return renderer;
    }
}
