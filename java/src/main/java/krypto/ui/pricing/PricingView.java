package krypto.ui.pricing;

import krypto.ui.UIDataCache;
import krypto.ui.components.HeatmapColumnTableCellRenderer;
import krypto.ui.components.LiveFrame;
import krypto.ui.components.TableColumnHeaderRenderer;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import javax.swing.border.EmptyBorder;

public class PricingView extends LiveFrame {

  private static final int NUMBER_OF_VALUES = 30;

  private final UIDataCache uiDataCache;
  private final TheoreticalSnapshotTable theoreticalSnapshotTable;
  private final TheoreticalSnapshotTableModel theoreticalSnapshotTableModel;
  private final PricingChartPanel chartPanel;

  public PricingView(final UIDataCache uiDataCache) {
    this.setTitle("PRICING");
    this.uiDataCache = uiDataCache;
    setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
    setBounds(100, 100, 800, 500);
    JPanel contentPane = new JPanel();
    contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
    setContentPane(contentPane);
    contentPane.setLayout(new MigLayout("", "[fill,grow]", "[fill,grow]"));

    final var tableScrollPane = new JScrollPane();
    final var theoCellRenderer = new HeatmapColumnTableCellRenderer();
    this.theoreticalSnapshotTableModel =
        new TheoreticalSnapshotTableModel(uiDataCache, theoCellRenderer);
    this.theoreticalSnapshotTable =
        new TheoreticalSnapshotTable(theoreticalSnapshotTableModel, theoCellRenderer);
    theoreticalSnapshotTable.getTableHeader().setDefaultRenderer(new TableColumnHeaderRenderer());
    tableScrollPane.setViewportView(theoreticalSnapshotTable);
    theoreticalSnapshotTable.setSelectionMode(ListSelectionModel.SINGLE_INTERVAL_SELECTION);
    theoreticalSnapshotTable
        .getSelectionModel()
        .addListSelectionListener(
            event -> this.updateProductSelection(theoreticalSnapshotTable.getSelectedRow()));

    this.chartPanel = new PricingChartPanel(NUMBER_OF_VALUES);

    final JSplitPane splitPane =
        new JSplitPane(JSplitPane.VERTICAL_SPLIT, tableScrollPane, chartPanel);
    splitPane.setDividerLocation(0.5);
    splitPane.setResizeWeight(0.5);
    contentPane.add(splitPane, "cell 0 0 1 1");
  }

  @Override
  protected void refreshUi() {
    SwingUtilities.invokeLater(
        () -> {
          if (this.uiDataCache.getActiveInstruments(false).size()
              != this.theoreticalSnapshotTableModel.getRowCount()) {
            this.theoreticalSnapshotTableModel.reinitialize();
          }
          this.theoreticalSnapshotTableModel.fireTableRowsUpdated(
              0, this.theoreticalSnapshotTableModel.getRowCount() - 1);
          this.theoreticalSnapshotTableModel.updateScaledTheoRatio();

          this.uiDataCache.getTheos().values().forEach(this.chartPanel::updateTheoSnapshot);
          this.uiDataCache.getQuotes().values().forEach(this.chartPanel::updateQuote);

          if (this.theoreticalSnapshotTable.getSelectedRows().length == 0) {
            this.theoreticalSnapshotTable.setRowSelectionInterval(0, 0);
            this.updateProductSelection(0);
          }
        });
  }

  private void updateProductSelection(final int selectedRow) {
    final String symbol =
        theoreticalSnapshotTableModel
            .getValueAt(selectedRow, TheoreticalSnapshotTable.Column.INSTRUMENT.ordinal())
            .toString();
    final var id = this.uiDataCache.getSymbolToInstrumentIdMapping().get(symbol);
    this.chartPanel.setSelectedInstrument(id);
  }
}
