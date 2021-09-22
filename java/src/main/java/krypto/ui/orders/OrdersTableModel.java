package krypto.ui.orders;

import krypto.orders.Order;
import krypto.serialization.OrderStatus;
import krypto.serialization.Side;
import krypto.serialization.TimeInForce;
import krypto.ui.components.ReadOnlyTableModel;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class OrdersTableModel extends ReadOnlyTableModel {

  private final Map<String, Order> orders = new HashMap<>();
  private final List<String> orderIds = new ArrayList<>();

  @Override
  public int getRowCount() {
    if (this.orderIds == null) {
      return 0;
    }
    return this.orderIds.size();
  }

  @Override
  public int getColumnCount() {
    return OrdersTable.Column.values().length;
  }

  @Override
  public String getColumnName(int column) {
    return OrdersTable.Column.values()[column].toString();
  }

  @Override
  public Object getValueAt(int row, int column) {
    if (this.orderIds.size() == 0) {
      return null;
    }
    final Order order = this.orders.get(this.orderIds.get(row));

    final OrdersTable.Column col = OrdersTable.Column.values()[column];

    switch (col) {
      case ORDER_ID:
        return order.getOrderId();
      case PRODUCT:
        return order.getSymbol();
      case PRICE:
        return order.getPrice();
      case SIZE:
        return order.getSize();
      case SIDE:
        return Side.name(order.getSide());
      case FILLED:
        return order.getFilledQty();
      case STATUS:
        return OrderStatus.name(order.getStatus());
      case TIMESTAMP:
        return order.getLastUpdatedTimestamp();
      case FEES:
        return order.getFees();
      case TIF:
        return TimeInForce.name(order.getTif());
    }

    return null;
  }

  public void updateOrders(final Map<String, Order> orders) {
    orders.forEach(
        (k, v) -> {
          if (!this.orders.containsKey(k)) {
            this.orderIds.add(v.getClOrderId());
            this.fireTableRowsInserted(this.getRowCount() - 1, this.getRowCount() - 1);
          }
          this.orders.put(k, v);
        });
    this.fireTableRowsUpdated(0, this.getRowCount() - 1);
  }
}
