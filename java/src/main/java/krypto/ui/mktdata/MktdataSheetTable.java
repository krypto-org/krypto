package krypto.ui.mktdata;

import krypto.ui.components.ColorConstants;
import krypto.ui.components.ColoredColumnTableCellRenderer;

import javax.swing.*;
import javax.swing.table.TableModel;
import java.awt.*;
import java.util.HashMap;
import java.util.Map;

public class MktdataSheetTable extends JTable {
    private static final Map<Column, Color> COLUMN_COLORS;

    static {
        COLUMN_COLORS = new HashMap<>();
        COLUMN_COLORS.put(Column.BID, ColorConstants.BID_PRICE_COLUMN_COLOR);
        COLUMN_COLORS.put(Column.ASK, ColorConstants.ASK_PRICE_COLUMN_COLOR);
        COLUMN_COLORS.put(Column.BID_QUANTITY, ColorConstants.BID_QUANTITY_COLUMN_COLOR);
        COLUMN_COLORS.put(Column.ASK_QUANTITY, ColorConstants.ASK_QUANTITY_COLUMN_COLOR);
        COLUMN_COLORS.put(Column.LAST, ColorConstants.TOTAL_COLOR);
        COLUMN_COLORS.put(Column.LAST_QUANTITY, ColorConstants.TOTAL_COLOR);
    }

    public MktdataSheetTable(TableModel dm) {
        super(dm);
        COLUMN_COLORS.forEach((column, color) -> {
            this.getColumnModel().getColumn(column.ordinal()).setCellRenderer(
                    new ColoredColumnTableCellRenderer(color, Color.WHITE, column.ordinal(), true));
        });

    }

    public enum Column {
        INSTRUMENT,
        BID_QUANTITY,
        BID,
        ASK,
        ASK_QUANTITY,
        LAST,
        LAST_QUANTITY
    }
}
