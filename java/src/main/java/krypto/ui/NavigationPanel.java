package krypto.ui;

import krypto.ui.components.ImageButton;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Arrays;

public class NavigationPanel extends JPanel {

    public static String ANALYTICS_ICON_PATH = "/option_analytics.png";
    public static String RISK_ICON_PATH = "/risk.png";
    public static String INSTRUMENTS_ICON_PATH = "/instruments.png";
    public static String MARKET_DATA_ICON_PATH = "/market_data.png";
    public static String SETTINGS_ICON_PATH = "/settings.png";
    public static String AUDIT_ICON_PATH = "/audit.png";
    public static String TRADE_SHEETS_ICON_PATH = "/trade_sheets.png";
    public static String STRATEGY_ICON_PATH = "/strategy.png";

    private JButton btnCurves;
    private JButton btnRisk;
    private JButton btnInstruments;
    private JButton btnMarketData;
    private JButton btnSettings;
    private JButton btnAudit;
    private JButton btnTradeSheets;


    public NavigationPanel() {
        this.setLayout(new MigLayout("", "[fill,grow]", "[fill,grow]"));
        this.initializeButtons();
    }

    private void initializeButtons()
    {
        btnInstruments = new ImageButton(
                Toolkit.getDefaultToolkit().getImage(
                        this.getClass().getResource(INSTRUMENTS_ICON_PATH)),
                40, 40);
        btnInstruments.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {

            }
        });
        btnInstruments.setToolTipText("Instruments");
        this.add(btnInstruments, "cell 0 0 1 1");

        btnMarketData = new ImageButton(
                Toolkit.getDefaultToolkit().getImage(
                        this.getClass().getResource(MARKET_DATA_ICON_PATH)),
                40, 40);
        btnMarketData.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
//                initializeMarketDataWindow();
            }
        });
        btnMarketData.setToolTipText("Market Data");
        this.add(btnMarketData, "cell 1 0 1 1");

        btnCurves = new ImageButton(
                Toolkit.getDefaultToolkit()
                        .getImage(this.getClass()
                                .getResource(ANALYTICS_ICON_PATH)),
                40, 40);
        btnCurves.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
//                initializeTheoAnalysisWindow();
            }
        });
        btnCurves.setToolTipText("Theo Server");
        this.add(btnCurves, "cell 2 0 1 1");

        btnTradeSheets = new ImageButton(
                Toolkit.getDefaultToolkit().getImage(
                        this.getClass().getResource(TRADE_SHEETS_ICON_PATH)),
                40, 40);
        btnTradeSheets.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
//                initializeTradeSheetsView();
            }
        });
        btnTradeSheets.setToolTipText("Trade Sheets");
        this.add(btnTradeSheets, "cell 3 0 1 1");

        btnRisk = new ImageButton(Toolkit.getDefaultToolkit()
                .getImage(this.getClass().getResource(RISK_ICON_PATH)), 40, 40);
        btnRisk.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {

//                initializeRiskView();
            }
        });
        btnRisk.setToolTipText("Risk");
        this.add(btnRisk, "cell 4 0 1 1");

        btnSettings = new ImageButton(
                Toolkit.getDefaultToolkit().getImage(
                        this.getClass().getResource(SETTINGS_ICON_PATH)),
                40, 40);
        btnSettings.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
//                initializeSettingsView();
            }
        });
        btnSettings.setToolTipText("Settings");
        this.add(btnSettings, "cell 5 0 1 1");

        btnAudit = new ImageButton(Toolkit.getDefaultToolkit().getImage(
                this.getClass().getResource(AUDIT_ICON_PATH)), 40, 40);
        btnAudit.addActionListener(new ActionListener()
        {
            @Override
            public void actionPerformed(ActionEvent e)
            {
//                initializeLogView();
            }
        });
        btnAudit.setToolTipText("Audit Log");
        this.add(btnAudit, "cell 6 0 1 1");

        btnCurves.setEnabled(true);
        btnRisk.setEnabled(true);
        btnInstruments.setEnabled(true);
        btnMarketData.setEnabled(true);
        btnSettings.setEnabled(true);
        btnAudit.setEnabled(true);
        btnTradeSheets.setEnabled(true);
   }

}
