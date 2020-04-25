package krypto.ui.instruments;

import krypto.serialization.Instrument;
import krypto.ui.UIDataCache;
import krypto.ui.components.ReadOnlyTableModel;

import java.util.HashMap;
import java.util.Map;

public class InstrumentsTableModel extends ReadOnlyTableModel {
    private final UIDataCache dataCache;

    private final Map<Long, Instrument> instruments = new HashMap<>();
    private final Map<Integer, Long> instrumentRowNums = new HashMap<>();

    InstrumentsTableModel(final UIDataCache dataCache) {
        this.dataCache = dataCache;
    }

    @Override
    public int getRowCount() {
        if (this.instrumentRowNums == null) {
            return 0;
        }
        return this.instrumentRowNums.size();
    }

    @Override
    public int getColumnCount() {
        return InstrumentsTable.Column.values().length;
    }

    @Override
    public String getColumnName(int column) {
        return InstrumentsTable.Column.values()[column].toString().replace(
                "_", " ");
    }

    @Override
    public Object getValueAt(int row, int column) {
        if (this.instrumentRowNums.containsKey(row)) {
            final long instId = this.instrumentRowNums.get(row);
            if (this.instruments.containsKey(instId)) {
                final InstrumentsTable.Column col =
                        InstrumentsTable.Column.values()[column];
                final Instrument instrument = this.instruments.get(instId);
                switch (col) {
                    case ID:
                        return instrument.id();
                    case INSTRUMENT_TYPE:
                        return instrument.type();
                    case SYMBOL:
                        return instrument.symbol();
                    case EXCHANGE:
                        return instrument.exchange();
                    case EXCHANGE_SYMBOL:
                        return instrument.exchangeSymbol();
                    case TICK_SIZE:
                        return instrument.tickSize();
                    case MIN_SIZE:
                        return instrument.minSize();
                    case MAX_SIZE:
                        return instrument.maxSize();
                    case CRYPTO_BASE:
                        return instrument.cryptoBase();
                    case CRYPTO_QUOTE:
                        return instrument.cryptoQuote();
                    case ACTIVE:
                        return instrument.active();
                    default:
                        return '-';
                }
            }
        }
        return '-';
    }

    public void reload() {
        this.instruments.putAll(
                this.dataCache.getInstruments(false));
        int row = 0;
        for (final Instrument instrument : this.instruments.values()) {
            this.instrumentRowNums.put(row++, instrument.id());
        }
        this.fireTableRowsUpdated(0, this.getRowCount() - 1);
    }
}
