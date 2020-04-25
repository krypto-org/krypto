package krypto.ui.pricing;

import javax.swing.*;

public class PricingChartPanel extends JPanel {
    // TODO: TimeSeriesCollection
    // TODO: Add rolling window
    // TODO: Add new value
    // TODO: Remove last value if past the time window
    // 5 Lines - Theo Bid, Theo Offer, Actual Bid, Actual Offer, Mid

    private final long window;
    private final long updateFrequency;

    public PricingChartPanel(final long window, final long updateFrequency) {
        super();
        this.window = window;
        this.updateFrequency = updateFrequency;
    }
}
