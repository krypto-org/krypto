package krypto.ui.components;

import javax.swing.*;
import java.awt.*;
import java.text.NumberFormat;

public class EditableCellRenderer extends CenterAlignedTableCellRenderer {
    private final NumberFormat formatter;

    public EditableCellRenderer(NumberFormat formatter) {
        this.formatter = formatter;
    }

    @Override
    public void setValue(Object value) {
        setText((value == null) ? "" : formatter.format(value));
    }
}
