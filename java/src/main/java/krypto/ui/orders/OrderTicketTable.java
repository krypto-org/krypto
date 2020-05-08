package krypto.ui.orders;

import krypto.ui.components.*;

import javax.swing.*;
import java.util.Arrays;

public class OrderTicketTable extends JTable {

    public OrderTicketTable(final OrderTicketTableModel model) {
        super(model);
        this.getColumnModel()
                .getColumn(Column.TIF.ordinal())
                .setCellEditor(new ComboBoxTableCellEditor<>(TimeInForce.values()));

        this.getColumnModel()
                .getColumn(Column.Side.ordinal())
                .setCellEditor(new ComboBoxTableCellEditor<>(Side.values()));

        final CenterAlignedTableCellRenderer centerAlignedTableCellRenderer =
                new CenterAlignedTableCellRenderer();
        this.getColumnModel()
                .getColumn(Column.Product.ordinal())
                .setCellRenderer(centerAlignedTableCellRenderer);
        this.getColumnModel()
                .getColumn(Column.Price.ordinal())
                .setCellRenderer(centerAlignedTableCellRenderer);
        this.getColumnModel()
                .getColumn(Column.Size.ordinal())
                .setCellRenderer(centerAlignedTableCellRenderer);
    }

    public enum Column {
        Product,
        Price,
        Size,
        Side,
        TIF
    }
}
