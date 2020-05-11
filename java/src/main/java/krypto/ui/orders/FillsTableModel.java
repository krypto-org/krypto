package krypto.ui.orders;

import krypto.ui.components.ReadOnlyTableModel;

public class FillsTableModel extends ReadOnlyTableModel {

    @Override
    public int getRowCount() {
        return 0;
    }

    @Override
    public int getColumnCount() {
        return FillsTable.Column.values().length;
    }

    @Override
    public String getColumnName(int column) {
        return FillsTable.Column.values()[column].toString();
    }
}
