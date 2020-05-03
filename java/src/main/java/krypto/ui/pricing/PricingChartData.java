package krypto.ui.pricing;

import org.jfree.data.time.TimeSeries;
import org.jfree.data.time.TimeSeriesCollection;

public class PricingChartData {
    private final TimeSeries mmBidSeries = new TimeSeries("MM Bid");

    private final TimeSeries mmAskSeries = new TimeSeries("MM Ask");
    private final TimeSeries bidSeries = new TimeSeries("Bid");
    private final TimeSeries askSeries = new TimeSeries("Ask");
    private final TimeSeries theoSeries = new TimeSeries("Theo");

    final TimeSeriesCollection dataset = new TimeSeriesCollection();

    public PricingChartData() {
        this.dataset.addSeries(this.mmBidSeries);
        this.dataset.addSeries(this.mmAskSeries);
        this.dataset.addSeries(this.bidSeries);
        this.dataset.addSeries(this.askSeries);
        this.dataset.addSeries(this.theoSeries);
    }

    public TimeSeries getMmBidSeries() {
        return mmBidSeries;
    }

    public TimeSeries getMmAskSeries() {
        return mmAskSeries;
    }

    public TimeSeries getBidSeries() {
        return bidSeries;
    }

    public TimeSeries getAskSeries() {
        return askSeries;
    }

    public TimeSeries getTheoSeries() {
        return theoSeries;
    }

    public TimeSeriesCollection getDataset() {
        return dataset;
    }
}
