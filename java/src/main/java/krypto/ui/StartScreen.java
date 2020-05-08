package krypto.ui;


import krypto.serialization.Instrument;
import krypto.ui.components.HeatmapColumnTableCellRenderer;
import krypto.ui.components.LiveFrame;
import krypto.ui.components.TableColumnHeaderRenderer;
import krypto.ui.mktdata.MktdataSheetTable;
import krypto.ui.mktdata.MktdataSheetTableModel;
import krypto.ui.orders.OrderTicketPanel;
import net.miginfocom.swing.MigLayout;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.swing.*;
import java.awt.*;
import java.util.SortedMap;
import java.util.concurrent.ConcurrentSkipListMap;

public class StartScreen extends LiveFrame
{
    private static final Logger logger = LogManager.getLogger(StartScreen.class);

    private static final String APPLICATION_ICON_PATH = "/krypto.png";

    private final UIDataCache uiDataCache;
    private final SortedMap<Long, Instrument> instruments;
    private final JTable quotesTable;
    private final MktdataSheetTableModel quotesTableModel;
    private final NavigationPanel navigationPanel;
    private final OrderTicketPanel orderTicketPanel;

    public StartScreen(final UIDataCache uiDataCache) {
        this.setTitle("KRYPTO");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setBounds(100, 100, 950, 750);

        this.setIconImage(Toolkit.getDefaultToolkit().getImage(
                StartScreen.class.getResource(APPLICATION_ICON_PATH)));

        JPanel contentPane = new JPanel();
        this.setContentPane(contentPane);
        contentPane.setLayout(new MigLayout("", "[fill,grow]", "[50][fill,grow][75]"));

        this.uiDataCache = uiDataCache;

        this.instruments = new ConcurrentSkipListMap<>();
        final var quotesScrollPane = new JScrollPane();
        final var theoCellRemderer = new HeatmapColumnTableCellRenderer();

        this.quotesTableModel = new MktdataSheetTableModel(theoCellRemderer);
        this.quotesTable = new MktdataSheetTable(this.quotesTableModel, theoCellRemderer);
        quotesTable.getTableHeader()
                .setDefaultRenderer(new TableColumnHeaderRenderer());
        quotesScrollPane.setViewportView(quotesTable);

        this.navigationPanel = new NavigationPanel(uiDataCache);
        this.orderTicketPanel = new OrderTicketPanel(uiDataCache);

        quotesTable.setSelectionMode(
                ListSelectionModel.SINGLE_INTERVAL_SELECTION);
        quotesTable.getSelectionModel().addListSelectionListener(
                event -> this.updateProductSelection(
                        quotesTable.getSelectedRow()));

        contentPane.add(navigationPanel, "wrap");
        contentPane.add(quotesScrollPane, "wrap");
        contentPane.add(this.orderTicketPanel);

        this.queryInstruments();
        this.startUpdates();
    }

    private void queryInstruments()
    {
        Thread instrumentsCallThread = new Thread(() ->
        {
            this.instruments.clear();
            this.uiDataCache.getActiveInstruments(true).forEach((this.instruments::put));
            logger.info(String.format("TOTAL INSTRUMENTS: %s", instruments.size()));
            this.onInstrumentsLoad();
            logger.info("LOADED INSTRUMENTS AND UPDATED MAPPING TABLE");
        });

        instrumentsCallThread.start();
    }

    private void onInstrumentsLoad() {
        this.quotesTableModel.updateInstruments(this.instruments);
        this.quotesTableModel.updateTable();
        SwingUtilities.invokeLater(
                this.navigationPanel::refresh);
    }

    @Override
    protected void refreshUi()
    {
        SwingUtilities.invokeLater(
                () -> {
                    this.quotesTableModel.updateQuotes(
                            this.uiDataCache.getQuotes());
                    this.quotesTableModel.updateTheos(
                            this.uiDataCache.getTheos());
                    this.quotesTableModel.updateScaledTheoRatio();
                    if (this.quotesTable.getSelectedRows().length == 0) {
                        this.quotesTable.setRowSelectionInterval(0, 0);
                        this.updateProductSelection(0);
                    }
                });
    }

    private void updateProductSelection(final int selectedRow) {
        final String symbol = quotesTableModel.getValueAt(
                selectedRow,
                MktdataSheetTable.Column.INSTRUMENT.ordinal()).toString();
        final var id = this.uiDataCache.getSymbolToInstrumentIdMapping().get(symbol);
        this.orderTicketPanel.setSelectedInstrument(id);
    }
}

