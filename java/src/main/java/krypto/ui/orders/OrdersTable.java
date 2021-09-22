package krypto.ui.orders;

import krypto.ui.components.CenterAlignedTableCellRenderer;
import krypto.ui.components.ColoredColumnTableCellRenderer;
import krypto.ui.components.TableColumnHeaderRenderer;

import javax.swing.*;
import javax.swing.table.TableModel;
import java.util.Arrays;
import java.util.stream.IntStream;

public class OrdersTable extends JTable {

  public OrdersTable(final TableModel model) {
    super(model);
    final var cellRenderer = new ActiveOrderTableCellRenderer();
    IntStream.range(0, Column.values().length)
        .forEach(
            idx -> {
              this.columnModel.getColumn(idx).setCellRenderer(cellRenderer);
            });
    this.getTableHeader().setDefaultRenderer(new TableColumnHeaderRenderer());
  }

  public enum Column {
    ORDER_ID,
    TIMESTAMP,
    PRODUCT,
    PRICE,
    SIZE,
    SIDE,
    TIF,
    STATUS,
    FILLED,
    FEES
  }
}
