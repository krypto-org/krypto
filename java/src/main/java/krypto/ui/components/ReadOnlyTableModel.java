package krypto.ui.components;

import javax.swing.table.DefaultTableModel;

public class ReadOnlyTableModel extends DefaultTableModel {
    @Override
    public boolean isCellEditable(int row, int column) {
        return false;
    }

    public void updateTable() {
        super.fireTableRowsUpdated(0, this.getRowCount() - 1);
    }
}