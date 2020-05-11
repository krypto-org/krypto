package krypto.ui.orders;

import krypto.orders.Fill;
import krypto.serialization.Side;
import krypto.ui.components.ReadOnlyTableModel;

import java.util.ArrayList;
import java.util.List;

public class FillsTableModel extends ReadOnlyTableModel {

    private final List<Fill> fills = new ArrayList<>();

    @Override
    public int getRowCount() {
        if (this.fills == null) {
            return 0;
        }
        return this.fills.size();
    }

    @Override
    public int getColumnCount() {
        return FillsTable.Column.values().length;
    }

    @Override
    public String getColumnName(int column) {
        return FillsTable.Column.values()[column].toString();
    }

    @Override
    public Object getValueAt(int row, int column) {
        if (this.fills.size() == 0) {
            return null;
        }

        final FillsTable.Column col = FillsTable.Column.values()[column];

        final Fill fill = this.fills.get(row);

        switch (col) {
            case ORDER_ID:
                return fill.getOrderId();
            case SIDE:
                return Side.name(fill.getSide());
            case PRICE:
                return fill.getPrice();
            case FILLED_QTY:
                return fill.getFilledQty();
            case REM_QTY:
                return fill.getRemainingQty();
        }
        return null;
    }

    public void addFill(final Fill fill) {
        this.fills.add(fill);
        this.fireTableRowsInserted(this.getRowCount() - 1, this.getRowCount() - 1);
        this.fireTableRowsUpdated(this.getRowCount() - 1, this.getRowCount() - 1);
    }
}
