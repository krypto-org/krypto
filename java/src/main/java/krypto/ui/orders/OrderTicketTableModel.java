package krypto.ui.orders;

import krypto.mktdata.Conversion;
import krypto.orders.Exchange;
import krypto.serialization.Quote;

import javax.swing.table.AbstractTableModel;

public class OrderTicketTableModel extends AbstractTableModel {

  private static final double MIN_SIZE = 0.001;

  private String symbol = "N/A";
  private double price = Double.NaN;
  private double size = Double.NaN;
  private Side side = Side.BUY;
  private TimeInForce tif = TimeInForce.DAY;
  private Exchange exchange = Exchange.SIM;

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
    return columnIndex != OrderTicketTable.Column.Product.ordinal();
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
        return side.toString();
      case TIF:
        return tif.toString();
      case EXCHANGE:
        return exchange.toString();
    }
    return null;
  }

  @Override
  public void setValueAt(Object value, int rowIndex, int columnIndex) {
    if (columnIndex == OrderTicketTable.Column.Side.ordinal()) {
      this.side = (Side) value;
    } else if (columnIndex == OrderTicketTable.Column.TIF.ordinal()) {
      this.tif = (TimeInForce) value;
    } else if (columnIndex == OrderTicketTable.Column.Price.ordinal()) {
      this.price = Double.parseDouble(value.toString());
    } else if (columnIndex == OrderTicketTable.Column.Size.ordinal()) {
      this.size = Double.parseDouble(value.toString());
    } else if (columnIndex == OrderTicketTable.Column.EXCHANGE.ordinal()) {
      this.exchange = Exchange.valueOf(value.toString());
    }
    super.setValueAt(value, rowIndex, columnIndex);
    this.fireTableRowsUpdated(rowIndex, rowIndex);
  }

  public void update(final String symbol, final Quote quote) {
    this.symbol = symbol;
    if (quote != null) {
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
    } else {
      this.price = Double.NaN;
    }
    this.size = MIN_SIZE;
    this.fireTableRowsUpdated(0, 0);
  }

  public String getSymbol() {
    return symbol;
  }

  public double getPrice() {
    return price;
  }

  public double getSize() {
    return size;
  }

  public Side getSide() {
    return side;
  }

  public TimeInForce getTif() {
    return tif;
  }

  public Exchange getExchange() {
    return exchange;
  }
}
