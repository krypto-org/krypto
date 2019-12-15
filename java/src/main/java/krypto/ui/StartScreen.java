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
import java.awt.*;
import java.util.SortedMap;
import java.util.concurrent.ConcurrentSkipListMap;

public class StartScreen extends LiveFrame
{
    private static final Logger logger = LogManager.getLogger(StartScreen.class);

    public static final String APPLICATION_ICON_PATH = "/krypto.png";

    private final UIDataCache uiDataCache;
    private final SortedMap<Long, Instrument> instruments;
    private final MktdataSheetTableModel quotesTableModel;
    private final JPanel navigationPanel;

    public StartScreen(final UIDataCache uiDataCache) {
        this.setTitle("KRYPTO");
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setBounds(100, 100, 950, 750);

        this.setIconImage(Toolkit.getDefaultToolkit().getImage(
                StartScreen.class.getResource(APPLICATION_ICON_PATH)));

        JPanel contentPane = new JPanel();
        this.setContentPane(contentPane);
        contentPane.setLayout(new MigLayout("", "[fill,grow]", "[50][fill,grow]"));

        this.uiDataCache = uiDataCache;

        this.instruments = new ConcurrentSkipListMap<>();
        JScrollPane quotesScrollPane = new JScrollPane();

        this.quotesTableModel = new MktdataSheetTableModel();
        JTable quotesTable = new MktdataSheetTable(this.quotesTableModel);
        quotesTable.getTableHeader()
                .setDefaultRenderer(new TableColumnHeaderRenderer());
        quotesScrollPane.setViewportView(quotesTable);

        this.navigationPanel = new NavigationPanel();

        contentPane.add(this.navigationPanel, "wrap");
        contentPane.add(quotesScrollPane);

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
        SwingUtilities.invokeLater(
                () -> {
                    this.quotesTableModel.updateQuotes(
                            this.uiDataCache.getQuotes());
                    this.quotesTableModel.updateTheos(
                            this.uiDataCache.getTheos());
                });
    }
}

