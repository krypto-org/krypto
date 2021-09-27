package krypto.ui.orders;

import krypto.orders.Exchange;
import krypto.ui.components.CenterAlignedTableCellRenderer;
import krypto.ui.components.ComboBoxTableCellEditor;
import krypto.ui.components.EditableCellRenderer;

import javax.swing.*;
import javax.swing.text.JTextComponent;
import java.awt.*;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.EventObject;

public class OrderTicketTable extends JTable {

  private static final NumberFormat PRICE_FORMAT = new DecimalFormat("0.0000");
  private static final NumberFormat QUANTITY_FORMAT = new DecimalFormat("0.000000");

  public OrderTicketTable(final OrderTicketTableModel model) {
    super(model);

    final CenterAlignedTableCellRenderer centerAlignedTableCellRenderer =
        new CenterAlignedTableCellRenderer();

    this.getColumnModel()
        .getColumn(Column.TIF.ordinal())
        .setCellEditor(new ComboBoxTableCellEditor<>(TimeInForce.values()));
    this.getColumnModel()
        .getColumn(Column.TIF.ordinal())
        .setCellRenderer(centerAlignedTableCellRenderer);

    this.getColumnModel()
        .getColumn(Column.Side.ordinal())
        .setCellEditor(new BuySellComboBoxCellEditor(Side.values()));
    this.getColumnModel()
        .getColumn(Column.Side.ordinal())
        .setCellRenderer(new BuySellTableCellRenderer());
    this.getColumnModel()
        .getColumn(Column.Product.ordinal())
        .setCellRenderer(centerAlignedTableCellRenderer);
    this.getColumnModel()
        .getColumn(Column.Trader.ordinal())
        .setCellRenderer(centerAlignedTableCellRenderer);
    this.getColumnModel()
        .getColumn(Column.Book.ordinal())
        .setCellRenderer(centerAlignedTableCellRenderer);
    this.getColumnModel()
        .getColumn(Column.Price.ordinal())
        .setCellRenderer(new EditableCellRenderer(PRICE_FORMAT));
    this.getColumnModel()
        .getColumn(Column.Size.ordinal())
        .setCellRenderer(new EditableCellRenderer(QUANTITY_FORMAT));

    this.getColumnModel()
        .getColumn(Column.Exchange.ordinal())
        .setCellEditor(new ComboBoxTableCellEditor<>(Exchange.values()));
    this.getColumnModel()
        .getColumn(Column.Exchange.ordinal())
        .setCellRenderer(centerAlignedTableCellRenderer);
    this.setRowSelectionAllowed(false);
    this.setColumnSelectionAllowed(false);
    this.setCellSelectionEnabled(true);
  }

  @Override
  public boolean editCellAt(int row, int column, EventObject e) {
    boolean result = super.editCellAt(row, column, e);
    final Component editor = getEditorComponent();
    if (!(editor instanceof JTextComponent)) {
      return result;
    }
    if (e != null) {
      ((JTextComponent) editor).selectAll();
    }
    return result;
  }

  public enum Column {
    Product,
    Price,
    Size,
    Side,
    TIF,
    Exchange,
    Trader,
    Book
  }
}
