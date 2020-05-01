package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;
import java.util.HashMap;
import java.util.Map;

public class HeatmapColumnTableCellRenderer extends DefaultTableCellRenderer {

    private static final Color BLUE = ColorConstants.CustomColors.APPLICATION_BLUE;
    private static final Color RED = ColorConstants.CustomColors.APPLICATION_RED;

    private static final double SCALE = 1.0;

    private final Map<Integer, Double> values;

    public HeatmapColumnTableCellRenderer() {
        values = new HashMap<>();
    }

    @Override
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        Component c = super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);

        if (this.values.containsKey(row)) {
            final double scaledValue = this.values.get(row);
            c.setBackground(interpolateBgdColorSigned(scaledValue));
            c.setForeground(interpolateFgdColorSigned(scaledValue));
        }

        return c;
    }

    @Override
    public int getHorizontalAlignment() {
        return JLabel.CENTER;
    }

    public void updateValues(final Map<Integer, Double> scaledvalues) {
        this.values.putAll(scaledvalues);
    }

    private static Color interpolateBgdColorSigned(final double z) {
        double ratio = Math.min(1.0, Math.abs(z / SCALE));
        Color result;
        int red;
        int green;
        int blue;
        if (z > 0) {
            red = (int) (255 * (1 - ratio) + RED.getRed() * ratio);
            green = (int) (255 * (1 - ratio) + RED.getGreen() * ratio);
            blue = (int) (255 * (1 - ratio) + RED.getBlue() * ratio);
        } else {
            red = (int) (255 * (1 - ratio) + BLUE.getRed() * ratio);
            green = (int) (255 * (1 - ratio) + BLUE.getGreen() * ratio);
            blue = (int) (255 * (1 - ratio) + BLUE.getBlue() * ratio);
        }
        result = new Color(red, green, blue);

        return result;
    }

    private static Color interpolateFgdColorSigned(final double z) {
        if (Math.abs(z) <= (SCALE / 2)) {
            return Color.BLACK;
        } else {
            return Color.WHITE;
        }
    }
    
}
