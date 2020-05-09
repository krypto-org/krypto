package krypto.ui;

import krypto.ui.components.ImageButton;
import krypto.ui.instruments.InstrumentsView;
import krypto.ui.pricing.PricingView;
import net.miginfocom.swing.MigLayout;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class NavigationPanel extends JPanel {

    private static final String PRICING_ICON_PATH = "/pricing.png";
    private static final String RISK_ICON_PATH = "/risk.png";
    private static final String INSTRUMENTS_ICON_PATH = "/instruments.png";
    private static final String MARKET_DATA_ICON_PATH = "/market_data.png";
    private static final String SETTINGS_ICON_PATH = "/settings.png";
    private static final String AUDIT_ICON_PATH = "/audit.png";
    private static final String TRADE_SHEETS_ICON_PATH = "/trade_sheets.png";
    public static final String STRATEGY_ICON_PATH = "/strategy.png";

    private final InstrumentsView instrumentsView;
    private final PricingView pricingView;
    private boolean instrumentsViewVisible = false;
    private boolean pricingViewVisible = false;

    NavigationPanel(final UIDataCache uiDataCache) {
        this.setLayout(new MigLayout("", "[fill,grow]", "[fill,grow]"));

        this.instrumentsView = new InstrumentsView(uiDataCache);
        this.pricingView = new PricingView(uiDataCache);

        this.initializeButtons();
    }

    private void initializeButtons() {
        final JButton btnInstruments = new ImageButton(
                Toolkit.getDefaultToolkit().getImage(
                        this.getClass().getResource(INSTRUMENTS_ICON_PATH)),
                40, 40);
        btnInstruments.addActionListener(e -> this.initializeInstrumentsView());
        btnInstruments.setToolTipText("Instruments");
        this.add(btnInstruments, "cell 0 0 1 1");

        final JButton btnMarketData = new ImageButton(
                Toolkit.getDefaultToolkit().getImage(
                        this.getClass().getResource(MARKET_DATA_ICON_PATH)),
                40, 40);
        btnMarketData.addActionListener(e -> initializeMarketDataWindow());
        btnMarketData.setToolTipText("Market Data");
        this.add(btnMarketData, "cell 1 0 1 1");

        final JButton btnPricing = new ImageButton(
                Toolkit.getDefaultToolkit()
                        .getImage(this.getClass()
                                .getResource(PRICING_ICON_PATH)),
                40, 40);
        btnPricing.addActionListener(e -> initializePricingWindow());
        btnPricing.setToolTipText("Pricing");
        this.add(btnPricing, "cell 2 0 1 1");

        final JButton btnStrategy = new ImageButton(
                Toolkit.getDefaultToolkit().getImage(
                        this.getClass().getResource(STRATEGY_ICON_PATH)),
                40, 40);
        btnStrategy.addActionListener(e -> initializeStrategyWindow());
        btnStrategy.setToolTipText("Trade Sheets");
        this.add(btnStrategy, "cell 3 0 1 1");

        final JButton btnRisk = new ImageButton(Toolkit.getDefaultToolkit()
                .getImage(this.getClass().getResource(RISK_ICON_PATH)), 40, 40);
        btnRisk.addActionListener(e -> initializeRiskWindow());
        btnRisk.setToolTipText("Risk");
        this.add(btnRisk, "cell 4 0 1 1");

        final JButton btnSettings = new ImageButton(
                Toolkit.getDefaultToolkit().getImage(
                        this.getClass().getResource(SETTINGS_ICON_PATH)),
                40, 40);
        btnSettings.addActionListener(e -> initializeSettingsWindow());
        btnSettings.setToolTipText("Settings");
        this.add(btnSettings, "cell 5 0 1 1");

        final JButton btnAudit = new ImageButton(Toolkit.getDefaultToolkit().getImage(
                this.getClass().getResource(AUDIT_ICON_PATH)), 40, 40);
        btnAudit.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                initializeLogView();
            }
        });
        btnAudit.setToolTipText("Audit Log");
        this.add(btnAudit, "cell 6 0 1 1");

        btnPricing.setEnabled(true);
        btnRisk.setEnabled(true);
        btnInstruments.setEnabled(true);
        btnMarketData.setEnabled(true);
        btnSettings.setEnabled(true);
        btnAudit.setEnabled(true);
        btnStrategy.setEnabled(true);
    }

    private void initializeInstrumentsView() {
        if (!instrumentsViewVisible) {
            instrumentsView.setIconImage(Toolkit.getDefaultToolkit().getImage(
                    NavigationPanel.class.getResource(INSTRUMENTS_ICON_PATH)));
            instrumentsViewVisible = true;
        }
        instrumentsView.setVisible(true);
    }

    private void initializeMarketDataWindow() {
    }

    private void initializePricingWindow() {
        if (!pricingViewVisible) {
            pricingView.setIconImage(Toolkit.getDefaultToolkit().getImage(
                    NavigationPanel.class.getResource(PRICING_ICON_PATH)));
            pricingViewVisible = true;
        }
        pricingView.setVisible(true);
    }

    private void initializeStrategyWindow() {

    }

    private void initializeRiskWindow() {

    }

    private void initializeSettingsWindow() {

    }

    private void initializeLogView() {

    }

    void refresh() {
        this.instrumentsView.refresh();
        this.pricingView.startUpdates();
    }
}
