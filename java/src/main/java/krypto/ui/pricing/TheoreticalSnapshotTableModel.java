package krypto.ui.pricing;

import krypto.serialization.Instrument;
import krypto.serialization.TheoreticalSnapshot;
import krypto.ui.DisplayConstants;
import krypto.ui.UIDataCache;
import krypto.ui.components.HeatmapColumnTableCellRenderer;
import krypto.ui.components.ReadOnlyTableModel;

import java.util.HashMap;
import java.util.Map;
import java.util.SortedMap;

public class TheoreticalSnapshotTableModel extends ReadOnlyTableModel {
    private final UIDataCache uiDataCache;
    private final Map<Integer, Long> instrumentIdToRowNums;

    private final HeatmapColumnTableCellRenderer theoCellRenderer;

    public TheoreticalSnapshotTableModel(
            final UIDataCache uiDataCache,
            final HeatmapColumnTableCellRenderer theoCellRenderer) {
        this.uiDataCache = uiDataCache;
        this.instrumentIdToRowNums = new HashMap<>();
        this.theoCellRenderer = theoCellRenderer;
    }

    @Override
    public int getColumnCount() {
        return TheoreticalSnapshotTable.Column.values().length;
    }

    @Override
    public String getColumnName(int column) {
        return TheoreticalSnapshotTable.Column.values()[column].toString();
    }

    public void updateScaledTheoRatio() {
        final Map<Integer, Double> theoScaledValues = new HashMap<>();
        for (int row = 0; row < this.getRowCount(); ++row) {
            final long instId = this.instrumentIdToRowNums.get(row);
            if (this.uiDataCache.getTheos().containsKey(instId) &&
                    this.uiDataCache.getTheos().containsKey(instId)) {
                final TheoreticalSnapshot theoSnapshot = this.uiDataCache.getTheos().get(instId);
                final double bid = theoSnapshot.mmBaseBid();
                final double ask = theoSnapshot.mmBaseAsk();
                final double diff = (ask - bid);
                final double value = (((Math.min(Math.max(theoSnapshot.price() - bid, 0), diff)) / diff) - 0.5) / 0.5;
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

    @Override
    public Object getValueAt(int row, int column) {
        if (this.instrumentIdToRowNums.containsKey(row)) {
            final long instId = this.instrumentIdToRowNums.get(row);
            final TheoreticalSnapshotTable.Column col =
                    TheoreticalSnapshotTable.Column.values()[column];
            if (col == TheoreticalSnapshotTable.Column.INSTRUMENT) {
                return this.uiDataCache.getActiveInstruments(false).get(
                        instId).symbol();
            }
            if (this.uiDataCache.getTheos().containsKey(instId)) {
                final TheoreticalSnapshot snapshot =
                        this.uiDataCache.getTheos().get(instId);
                switch (col) {
                    case THEO:
                        return DisplayConstants.PRICE_FORMAT.format(
                                snapshot.price());
                    case MM_BID:
                        return DisplayConstants.PRICE_FORMAT.format(
                                snapshot.mmBaseBid());
                    case MM_ASK:
                        return DisplayConstants.PRICE_FORMAT.format(
                                snapshot.mmBaseAsk());
                    case ERROR:
                        return DisplayConstants.PRICE_FORMAT.format(
                                snapshot.error());
                    case BID_LIQUIDITY:
                        return DisplayConstants.QUANTITY_FORMAT.format(snapshot.bidLiquidity());
                    case ASK_LIQUIDITY:
                        return DisplayConstants.QUANTITY_FORMAT.format(snapshot.askLiquidity());
                    case ADJUSTED_THEO:
                        return DisplayConstants.PRICE_FORMAT.format(
                                snapshot.adjustedPrice());
                    case WIDTH_MULT:
                        return DisplayConstants.MULTIPLIER_FORMAT.format(
                                snapshot.mmBaseSpread() / snapshot.error());
                    default:
                        return Double.NaN;
                }
            }
        }
        return Double.NaN;
    }

    public void reinitialize() {
        int row = 0;
        for (final Instrument inst : this.uiDataCache
                .getInstruments(false).values()) {
            if (inst.active()) {
                this.instrumentIdToRowNums.put(row++, inst.id());
            }
        }
        this.fireTableRowsUpdated(0, this.getRowCount() - 1);
    }
}
