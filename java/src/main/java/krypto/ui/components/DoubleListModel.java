package krypto.ui.components;

import javax.swing.*;

public class DoubleListModel extends AbstractListModel<Double> {
    private final Object[] values;

    public DoubleListModel(Object[] objects) {
        this.values = objects;
    }

    @Override
    public int getSize() {
        return values.length;
    }

    @Override
    public Double getElementAt(int index) {
        return Double.parseDouble(values[index].toString());
    }
}
