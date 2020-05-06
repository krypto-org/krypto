package krypto.ui.orders;

import krypto.serialization.TimeInForce;
import krypto.ui.components.CenterAlignedTableCellRenderer;
import krypto.ui.components.ComboBoxTableCellEditor;
import krypto.ui.components.ComboBoxTableCellRenderer;
import krypto.ui.components.ToggleTableCellRenderer;

import javax.swing.*;
import java.util.Arrays;

public class OrderTicketTable extends JTable {

    public OrderTicketTable(final OrderTicketTableModel model) {
        super(model);
        this.getColumnModel()
                .getColumn(Column.Side.ordinal())
                .setCellRenderer(new ToggleTableCellRenderer(Column.Side.ordinal(), "BUY", "SELL"));
        this.getColumnModel()
                .getColumn(Column.TIF.ordinal())
                .setCellRenderer(new ComboBoxTableCellRenderer<Byte>(TimeInForce::name));
        this.getColumnModel()
                .getColumn(Column.TIF.ordinal())
                .setCellEditor(
                        new ComboBoxTableCellEditor<Byte>(
                                Arrays.asList(
                                        TimeInForce.DAY,
                                        TimeInForce.IOC,
                                        TimeInForce.FOK,
                                        TimeInForce.GTC,
                                        TimeInForce.GTT),
                                TimeInForce::name));
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
