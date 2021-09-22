package krypto.ui.pricing;

import krypto.ui.components.ColorConstants;
import krypto.ui.components.ColoredColumnTableCellRenderer;

import javax.swing.*;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableModel;
import java.awt.*;
import java.util.HashMap;
import java.util.Map;

public class TheoreticalSnapshotTable extends JTable {

  private static final Map<Column, Color> COLUMN_COLORS;

  static {
    COLUMN_COLORS = new HashMap<>();
    COLUMN_COLORS.put(Column.MM_BID, ColorConstants.THEO_BID_BACKGROUND_COLOR);
    COLUMN_COLORS.put(Column.MM_ASK, ColorConstants.THEO_ASK_BACKGROUND_COLOR);
    COLUMN_COLORS.put(Column.BID_LIQUIDITY, ColorConstants.THEO_BID_BACKGROUND_COLOR);
    COLUMN_COLORS.put(Column.ASK_LIQUIDITY, ColorConstants.THEO_ASK_BACKGROUND_COLOR);
    COLUMN_COLORS.put(Column.ERROR, ColorConstants.TOTAL_COLOR);
    COLUMN_COLORS.put(Column.WIDTH_MULT, ColorConstants.TOTAL_COLOR);
  }

  public TheoreticalSnapshotTable(final TableModel dm, final TableCellRenderer theoCellRenderer) {
    super(dm);
    COLUMN_COLORS.forEach(
        (column, color) -> {
          this.getColumnModel()
              .getColumn(column.ordinal())
              .setCellRenderer(
                  new ColoredColumnTableCellRenderer(color, Color.WHITE, column.ordinal(), true));
        });
    this.getColumnModel().getColumn(Column.THEO.ordinal()).setCellRenderer(theoCellRenderer);
    this.getColumnModel()
        .getColumn(Column.ADJUSTED_THEO.ordinal())
        .setCellRenderer(theoCellRenderer);
  }

  public enum Column {
    INSTRUMENT,
    BID_LIQUIDITY,
    MM_BID,
    THEO,
    ADJUSTED_THEO,
    MM_ASK,
    ASK_LIQUIDITY,
    ERROR,
    WIDTH_MULT
  }
}
