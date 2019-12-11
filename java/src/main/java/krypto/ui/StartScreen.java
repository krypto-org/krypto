package krypto.ui;


import krypto.serialization.Instrument;
import krypto.ui.components.LiveFrame;
import krypto.ui.components.TableColumnHeaderRenderer;
import krypto.ui.mktdata.MktdataSheetTable;
import krypto.ui.mktdata.MktdataSheetTableModel;
import net.miginfocom.swing.MigLayout;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;

import javax.swing.*;
import java.util.SortedMap;
import java.util.concurrent.ConcurrentSkipListMap;

public class StartScreen extends LiveFrame
{
    private static Logger logger = LogManager.getLogger(StartScreen.class);

    private final UIDataCache uiDataCache;
    private final SortedMap<Long, Instrument> instruments;
    private final MktdataSheetTableModel quotesTableModel;
    private final JMenuBar menuBar;

    public StartScreen(final UIDataCache uiDataCache) {
        this.setTitle("KRYPTO");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setBounds(100, 100, 950, 750);

        JPanel contentPane = new JPanel();
        this.setContentPane(contentPane);
        contentPane.setLayout(new MigLayout("", "[fill,grow]", "[fill,grow]"));

        this.uiDataCache = uiDataCache;

        this.instruments = new ConcurrentSkipListMap<>();
        JScrollPane quotesScrollPane = new JScrollPane();

        this.quotesTableModel = new MktdataSheetTableModel();
        JTable quotesTable = new MktdataSheetTable(this.quotesTableModel);
        quotesTable.getTableHeader()
                .setDefaultRenderer(new TableColumnHeaderRenderer());
        quotesScrollPane.setViewportView(quotesTable);

        this.menuBar = new JMenuBar();
        final JMenu mdMenu = new JMenu("Mktdata");
        final JMenu ordersMenu = new JMenu("Orders");
        final JMenu riskMenu = new JMenu("Risk");
        final JMenu strategyMenu = new JMenu("Strategy");

        mdMenu.add(new JMenuItem("Ladder"));
        ordersMenu.add("Orders");
        ordersMenu.add("Fills");

        this.menuBar.add(mdMenu);
        this.menuBar.add(ordersMenu);
        this.menuBar.add(riskMenu);
        this.menuBar.add(strategyMenu);

        this.setJMenuBar(menuBar);

        contentPane.add(quotesScrollPane, "cell 0 0,grow");

        this.queryInstruments();
        this.startUpdates();
    }

    private void queryInstruments()
    {
        Thread instrumentsCallThread = new Thread(() ->
        {
            this.instruments.clear();
            this.uiDataCache.getInstruments(true).forEach((this.instruments::put));
            logger.info(String.format("TOTAL INSTRUMENTS: %s", instruments.size()));
            this.onInstrumentsLoad();
            logger.info("LOADED INSTRUMENTS AND UPDATED MAPPING TABLE");
        });

        instrumentsCallThread.start();
    }

    private void onInstrumentsLoad() {
        this.quotesTableModel.updateInstruments(this.instruments);
        this.quotesTableModel.updateTable();
    }

    @Override
    protected void refreshUi()
    {
        SwingUtilities.invokeLater(() -> this.quotesTableModel.updateQuotes(this.uiDataCache.getQuotes()));
    }
}

