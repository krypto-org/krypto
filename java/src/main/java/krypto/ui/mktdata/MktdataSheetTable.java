package krypto.ui.mktdata;

import krypto.ui.components.ColorConstants;
import krypto.ui.components.ColoredColumnTableCellRenderer;

import javax.swing.*;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableModel;
import java.awt.*;
import java.util.HashMap;
import java.util.Map;

public class MktdataSheetTable extends JTable {
  private static final Map<Column, Color> COLUMN_COLORS;

  static {
    COLUMN_COLORS = new HashMap<>();
    COLUMN_COLORS.put(Column.BID, ColorConstants.BID_BACKGROUND_COLOR);
    COLUMN_COLORS.put(Column.ASK, ColorConstants.ASK_BACKGROUND_COLOR);
    COLUMN_COLORS.put(Column.BID_QUANTITY, ColorConstants.BID_BACKGROUND_COLOR);
    COLUMN_COLORS.put(Column.ASK_QUANTITY, ColorConstants.ASK_BACKGROUND_COLOR);
    COLUMN_COLORS.put(Column.LAST, ColorConstants.TOTAL_COLOR);
    COLUMN_COLORS.put(Column.LAST_QUANTITY, ColorConstants.TOTAL_COLOR);
  }

  public MktdataSheetTable(TableModel dm, TableCellRenderer theoCellRenderer) {
    super(dm);
    COLUMN_COLORS.forEach(
        (column, color) -> {
          this.getColumnModel()
              .getColumn(column.ordinal())
              .setCellRenderer(
                  new ColoredColumnTableCellRenderer(color, Color.WHITE, column.ordinal(), true));
        });
    this.getColumnModel().getColumn(Column.THEO.ordinal()).setCellRenderer(theoCellRenderer);
  }

  public enum Column {
    INSTRUMENT,
    BID_QUANTITY,
    BID,
    THEO,
    ASK,
    ASK_QUANTITY,
    LAST,
    LAST_QUANTITY
  }
}
