package krypto.ui.orders;

import krypto.ui.components.CenterAlignedTableCellRenderer;
import krypto.ui.components.TableColumnHeaderRenderer;

import javax.swing.*;
import javax.swing.table.TableModel;
import java.util.stream.IntStream;

public class FillsTable extends JTable {
  public FillsTable(final TableModel model) {
    super(model);
    final var cellRenderer = new FillCellRenderer();
    IntStream.range(0, Column.values().length)
        .forEach(
            idx -> {
              this.columnModel.getColumn(idx).setCellRenderer(cellRenderer);
            });
    this.getTableHeader().setDefaultRenderer(new TableColumnHeaderRenderer());
  }

  public enum Column {
    ORDER_ID,
    SIDE,
    PRICE,
    FILLED_QTY,
    REM_QTY
  }
}
