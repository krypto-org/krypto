package krypto.ui.orders;

import krypto.mktdata.Conversion;
import krypto.serialization.Quote;
import krypto.serialization.Side;
import krypto.serialization.TimeInForce;

import javax.swing.table.AbstractTableModel;

public class OrderTicketTableModel extends AbstractTableModel {

    private static final double MIN_SIZE = 0.0001;

    private String symbol = "N/A";
    private double price = Double.NaN;
    private double size = Double.NaN;
    private byte side = Side.BUY;
    private byte tif = TimeInForce.DAY;

    @Override
    public int getRowCount() {
        return 1;
    }

    @Override
    public String getColumnName(int column) {
        return OrderTicketTable.Column.values()[column].toString();
    }

    @Override
    public int getColumnCount() {
        return OrderTicketTable.Column.values().length;
    }

    @Override
    public boolean isCellEditable(int rowIndex, int columnIndex) {
        return true;
    }

    @Override
    public Object getValueAt(int rowIndex, int columnIndex) {
        final OrderTicketTable.Column column = OrderTicketTable.Column.values()[columnIndex];
        switch (column) {
            case Product:
                return symbol;
            case Price:
                return price;
            case Size:
                return size;
            case Side:
                return Side.name(side);
            case TIF:
                return TimeInForce.name(tif);
        }
        return null;
    }

    @Override
    public void setValueAt(Object value, int rowIndex, int columnIndex) {
        if (columnIndex == OrderTicketTable.Column.Side.ordinal()) {
            if (value instanceof Byte) {
                this.side = (byte) value;
            }
        }
        else if (columnIndex == OrderTicketTable.Column.TIF.ordinal()) {
            if (value instanceof Byte) {
                this.tif = (byte) value;
            }
        }
        super.setValueAt(value, rowIndex, columnIndex);
        this.fireTableRowsUpdated(rowIndex, rowIndex);
    }

    public void update(final String symbol, final Quote quote) {
        this.symbol = symbol;
        if (tif == TimeInForce.DAY) {
            if (this.side == Side.BUY) {
                this.price = Conversion.convertPrice(quote.bid());
            } else {
                this.price = Conversion.convertPrice(quote.ask());
            }
        } else {
            if (this.side == Side.BUY) {
                this.price = Conversion.convertPrice(quote.ask());
            } else {
                this.price = Conversion.convertPrice(quote.bid());
            }
        }
        this.size = MIN_SIZE;
    }
}
