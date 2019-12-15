package krypto.ui.instruments;

import krypto.ui.components.CenterAlignedTableCellRenderer;
import krypto.ui.components.ColoredColumnTableCellRenderer;

import javax.swing.*;
import javax.swing.table.TableModel;
import java.util.Arrays;

class InstrumentsTable extends JTable {

    InstrumentsTable(TableModel dm) {
        super(dm);
        Arrays.stream(Column.values()).forEach(
                (column) ->
                        this.getColumnModel().getColumn(
                                column.ordinal()).setCellRenderer(
                new CenterAlignedTableCellRenderer()));
    }

    public enum Column {
        ID,
        INSTRUMENT_TYPE,
        SYMBOL,
        EXCHANGE,
        EXCHANGE_SYMBOL,
        TICK_SIZE,
        MIN_SIZE,
        MAX_SIZE,
        CRYPTO_BASE,
        CRYPTO_QUOTE
    }
}
