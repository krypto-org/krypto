package krypto.ui.pricing;


import krypto.ui.UIDataCache;
import krypto.ui.components.HeatmapColumnTableCellRenderer;
import krypto.ui.components.LiveFrame;
import krypto.ui.components.TableColumnHeaderRenderer;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import javax.swing.border.EmptyBorder;

public class PricingView extends LiveFrame {

    private final UIDataCache uiDataCache;
    private final TheoreticalSnapshotTableModel theoreticalSnapshotTableModel;

    public PricingView(final UIDataCache uiDataCache)  {
        this.setTitle("PRICING");
        this.uiDataCache = uiDataCache;
        setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        setBounds(100, 100, 800, 500);
        JPanel contentPane = new JPanel();
        contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
        setContentPane(contentPane);
        contentPane.setLayout(
                new MigLayout("",
                        "[fill,grow]",
                        "[fill,grow]"));

        final var tableScrollPane = new JScrollPane();
        final var theoCellRenderer = new HeatmapColumnTableCellRenderer();
        this.theoreticalSnapshotTableModel = new TheoreticalSnapshotTableModel(
                uiDataCache, theoCellRenderer);
        final var theoreticalSnapshotTable = new TheoreticalSnapshotTable(
                theoreticalSnapshotTableModel, theoCellRenderer);
        theoreticalSnapshotTable.getTableHeader()
                .setDefaultRenderer(new TableColumnHeaderRenderer());
        tableScrollPane.setViewportView(theoreticalSnapshotTable);

        contentPane.add(tableScrollPane, "cell 0 0 1 1");
    }

    @Override
    protected void refreshUi() {
        SwingUtilities.invokeLater(
                () -> {
                    if (this.uiDataCache.getActiveInstruments(false).size() !=
                            this.theoreticalSnapshotTableModel.getRowCount()) {
                        this.theoreticalSnapshotTableModel.reinitialize();
                    }
                    this.theoreticalSnapshotTableModel
                            .fireTableRowsUpdated(
                                    0, this.theoreticalSnapshotTableModel.getRowCount() - 1);
                    this.theoreticalSnapshotTableModel.updateScaledTheoRatio();
                });
    }
}
