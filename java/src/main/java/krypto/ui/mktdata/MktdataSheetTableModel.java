package krypto.ui.mktdata;

import krypto.mktdata.Conversion;
import krypto.network.MessageType;
import krypto.serialization.Instrument;
import krypto.serialization.Quote;
import krypto.serialization.TheoreticalSnapshot;
import krypto.ui.components.LiveUpdatedTableModel;
import krypto.ui.components.ReadOnlyTableModel;

import java.text.DecimalFormat;
import java.util.HashMap;
import java.util.Map;
import java.util.SortedMap;

public class MktdataSheetTableModel extends ReadOnlyTableModel {

    private final DecimalFormat PRICE_FORMAT = new DecimalFormat("0.0000");
    private final DecimalFormat QUANTITY_FORMAT = new DecimalFormat("0.0000");

    private final Map<Long, Instrument> instruments;
    private final Map<Long, Quote> quotes;
    private final Map<Long, TheoreticalSnapshot> theos;
    private final Map<Integer, Long> instrumentIdToRowNums;

    public MktdataSheetTableModel() {
        this.instruments = new HashMap<>();
        this.instrumentIdToRowNums = new HashMap<>();
        this.quotes = new HashMap<>();
        this.theos = new HashMap<>();
    }

    @Override
    public int getColumnCount() {
        return MktdataSheetTable.Column.values().length;
    }

    @Override
    public String getColumnName(int column) {
        return MktdataSheetTable.Column.values()[column].toString();
    }

    public void updateInstruments(final SortedMap<Long, Instrument> instruments) {
        instruments.forEach(this.instruments::put);
        int row = 0;
        for (final Instrument inst: this.instruments.values()) {
            this.instrumentIdToRowNums.put(row++, inst.id());
        }
        this.fireTableRowsUpdated(0, this.getRowCount() - 1);
    }

    @Override
    public Object getValueAt(int row, int column) {
        if (this.instrumentIdToRowNums.containsKey(row)) {
            final long instId = this.instrumentIdToRowNums.get(row);
            final MktdataSheetTable.Column col = MktdataSheetTable.Column.values()[column];
            if (col == MktdataSheetTable.Column.INSTRUMENT) {
                return this.instruments.get(instId).symbol();
            }
            if (this.quotes.containsKey(instId)) {
                final Quote quote = this.quotes.get(instId);
                switch (col) {
                    case BID:
                        return PRICE_FORMAT.format(Conversion.convertPrice(quote.bid()));
                    case BID_QUANTITY:
                        return QUANTITY_FORMAT.format(Conversion.convertQuantity(quote.bidQuantity()));
                    case THEO:
                        return this.theos.containsKey(instId) ?
                                PRICE_FORMAT.format(
                                        this.theos.get(instId).price()) : Double.NaN;
                    case ASK:
                        return PRICE_FORMAT.format(Conversion.convertPrice(quote.ask()));
                    case ASK_QUANTITY:
                        return QUANTITY_FORMAT.format(Conversion.convertQuantity(quote.askQuantity()));
                    case LAST:
                        return PRICE_FORMAT.format(Conversion.convertPrice(quote.last()));
                    case LAST_QUANTITY:
                        return QUANTITY_FORMAT.format(Conversion.convertQuantity(quote.lastQuantity()));
                    default:
                        return Double.NaN;
                }
            }

        }
        return Double.NaN;
    }

    public void updateQuotes(final Map<Long, Quote> quotes) {
        this.quotes.putAll(quotes);
        this.fireTableRowsUpdated(0, this.getRowCount() - 1);
    }

    public void updateTheos(final Map<Long, TheoreticalSnapshot> snapshots) {
        this.theos.putAll(snapshots);
        this.fireTableRowsUpdated(0, this.getRowCount());
    }

    @Override
    public int getRowCount() {
        if (this.instrumentIdToRowNums == null) {
            return 0;
        }
        return this.instrumentIdToRowNums.size();
    }
}
