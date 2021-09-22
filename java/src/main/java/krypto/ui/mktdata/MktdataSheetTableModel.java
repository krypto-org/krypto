package krypto.ui.mktdata;

import krypto.mktdata.Conversion;
import krypto.network.MessageType;
import krypto.serialization.Instrument;
import krypto.serialization.Quote;
import krypto.serialization.TheoreticalSnapshot;
import krypto.ui.DisplayConstants;
import krypto.ui.components.HeatmapColumnTableCellRenderer;
import krypto.ui.components.LiveUpdatedTableModel;
import krypto.ui.components.ReadOnlyTableModel;

import javax.swing.table.TableCellRenderer;
import java.text.DecimalFormat;
import java.util.HashMap;
import java.util.Map;
import java.util.SortedMap;

public class MktdataSheetTableModel extends ReadOnlyTableModel {

  private final Map<Long, Instrument> instruments;
  private final Map<Long, Quote> quotes;
  private final Map<Long, TheoreticalSnapshot> theos;
  private final Map<Integer, Long> instrumentIdToRowNums;

  private final HeatmapColumnTableCellRenderer theoCellRenderer;

  public MktdataSheetTableModel(final HeatmapColumnTableCellRenderer theoCellRenderer) {
    this.instruments = new HashMap<>();
    this.instrumentIdToRowNums = new HashMap<>();
    this.quotes = new HashMap<>();
    this.theos = new HashMap<>();

    this.theoCellRenderer = theoCellRenderer;
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
    this.instruments.putAll(instruments);
    int row = 0;
    for (final Instrument inst : this.instruments.values()) {
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
            return DisplayConstants.PRICE_FORMAT.format(Conversion.convertPrice(quote.bid()));
          case BID_QUANTITY:
            return DisplayConstants.QUANTITY_FORMAT.format(
                Conversion.convertQuantity(quote.bidQuantity()));
          case THEO:
            return this.theos.containsKey(instId)
                ? DisplayConstants.PRICE_FORMAT.format(this.theos.get(instId).price())
                : Double.NaN;
          case ASK:
            return DisplayConstants.PRICE_FORMAT.format(Conversion.convertPrice(quote.ask()));
          case ASK_QUANTITY:
            return DisplayConstants.QUANTITY_FORMAT.format(
                Conversion.convertQuantity(quote.askQuantity()));
          case LAST:
            return DisplayConstants.PRICE_FORMAT.format(Conversion.convertPrice(quote.last()));
          case LAST_QUANTITY:
            return DisplayConstants.QUANTITY_FORMAT.format(
                Conversion.convertQuantity(quote.lastQuantity()));
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
    this.fireTableRowsUpdated(0, this.getRowCount() - 1);
  }

  public void updateScaledTheoRatio() {
    final Map<Integer, Double> theoScaledValues = new HashMap<>();
    for (int row = 0; row < this.getRowCount(); ++row) {
      final long instId = this.instrumentIdToRowNums.get(row);
      if (this.quotes.containsKey(instId) && this.theos.containsKey(instId)) {
        final Quote quote = this.quotes.get(instId);
        final TheoreticalSnapshot theoSnapshot = this.theos.get(instId);

        final double bid = Conversion.convertPrice(quote.bid());
        final double ask = Conversion.convertPrice(quote.ask());
        final double diff = (ask - bid);

        final double value =
            (((Math.min(Math.max(theoSnapshot.price() - bid, 0), diff)) / diff) - 0.5) / 0.5;
        theoScaledValues.put(row, value);
      }
    }
    this.theoCellRenderer.updateValues(theoScaledValues);
  }

  @Override
  public int getRowCount() {
    if (this.instrumentIdToRowNums == null) {
      return 0;
    }
    return this.instrumentIdToRowNums.size();
  }
}
