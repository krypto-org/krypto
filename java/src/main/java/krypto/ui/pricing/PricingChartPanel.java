package krypto.ui.pricing;

import krypto.mktdata.Conversion;
import krypto.serialization.Quote;
import krypto.serialization.TheoreticalSnapshot;
import krypto.ui.components.ColorConstants;
import net.miginfocom.swing.MigLayout;
import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.time.Millisecond;
import org.jfree.data.time.Second;
import org.jfree.data.time.TimeSeries;
import org.jfree.data.time.TimeSeriesCollection;
import org.jfree.data.xy.XYDataset;

import javax.swing.*;
import java.awt.*;
import java.util.Date;

public class PricingChartPanel extends JPanel {
    // TODO: TimeSeriesCollection
    // TODO: Add rolling window
    // TODO: Add new value
    // TODO: Remove last value if past the time window
    // 5 Lines - Theo Bid, Theo Offer, Actual Bid, Actual Offer, Mid

    private final TimeSeries mmBidSeries;
    private final TimeSeries mmAskSeries;
    private final TimeSeries bidSeries;
    private final TimeSeries askSeries;
    private final TimeSeries theoSeries;

    private long initialTimestamp;

    public PricingChartPanel() {

        this.mmBidSeries = new TimeSeries("MM Bid");
        this.mmAskSeries = new TimeSeries("MM Ask");
        this.bidSeries = new TimeSeries("Bid");
        this.askSeries = new TimeSeries("Ask");
        this.theoSeries = new TimeSeries("Theo");

        final TimeSeriesCollection dataset = new TimeSeriesCollection();
        dataset.addSeries(this.mmBidSeries);
        dataset.addSeries(this.mmAskSeries);
        dataset.addSeries(this.bidSeries);
        dataset.addSeries(this.askSeries);
        dataset.addSeries(this.theoSeries);

        this.setLayout(
                new MigLayout("",
                        "[fill,grow]",
                        "[fill,grow]"));

        JFreeChart chart = createChart(dataset);
        final ChartPanel chartPanel = new ChartPanel(chart);
        this.add(chartPanel, "cell 0 0 1 1");
    }

    public void updateTheoSnapshot(final TheoreticalSnapshot snapshot) {
        final Millisecond now = new Millisecond(new Date(snapshot.timestamp() / 1000000));
        this.mmBidSeries.addOrUpdate(now, snapshot.mmBaseBid());
        this.mmAskSeries.addOrUpdate(now, snapshot.mmBaseAsk());
        this.theoSeries.addOrUpdate(now, snapshot.price());
    }

    public void updateQuote(final Quote quote) {
        final Millisecond now = new Millisecond(new Date(quote.timestamp() / 1000000));
        this.bidSeries.addOrUpdate(now, Conversion.convertPrice(quote.bid()));
        this.askSeries.addOrUpdate(now, Conversion.convertPrice(quote.ask()));
    }

    private JFreeChart createChart(final TimeSeriesCollection dataset) {
        final JFreeChart result = ChartFactory.createTimeSeriesChart(
                "Theo Snapshot",
                "Time",
                "Price",
                dataset, false,
                false,
                false
        );

        final XYPlot plot = result.getXYPlot();
        plot.getRenderer().setSeriesPaint(0,
                ColorConstants.THEO_BID_BACKGROUND_COLOR);
        plot.getRenderer().setSeriesPaint(1,
                ColorConstants.THEO_ASK_BACKGROUND_COLOR);
        plot.getRenderer().setSeriesPaint(2,
                ColorConstants.BID_BACKGROUND_COLOR);
        plot.getRenderer().setSeriesPaint(3,
                ColorConstants.ASK_BACKGROUND_COLOR);
        plot.getRenderer().setSeriesPaint(4,
                ColorConstants.CustomColors.APPLICATION_GOLDEN);

        ValueAxis valueAxis = plot.getDomainAxis();
        valueAxis.setAutoRange(true);

        return result;
    }

}
