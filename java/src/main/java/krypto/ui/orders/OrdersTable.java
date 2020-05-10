package krypto.ui.orders;

import javax.swing.*;
import javax.swing.table.TableModel;

public class OrdersTable extends JTable {

    public OrdersTable(final TableModel model) {
        super(model);
    }

    public enum Column {
        Timestamp,
        Product,
        Price,
        Size,
        Side,
        TIF,
        Status,
        Filled,
        OrderId,
        Fees
    }
}
