package krypto.ui.pricing;

import krypto.mktdata.Conversion;
import krypto.serialization.Quote;
import krypto.serialization.TheoreticalSnapshot;
import krypto.ui.components.ColorConstants;
import net.miginfocom.swing.MigLayout;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.time.Millisecond;

import javax.swing.*;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

public class PricingChartPanel extends JPanel {

    private final Map<Long, PricingChartData> dataByInstrument = new HashMap<>();
    private final JFreeChart chart = createChart();
    private final int numberOfValues;
    private final long timeframe;

    private long selectedInstrumentId;
    private long initialTimestamp;

    public PricingChartPanel(final int numberOfValues, final long timeframe) {
        this.numberOfValues = numberOfValues;
        this.timeframe = timeframe;
        this.setLayout(
                new MigLayout("",
                        "[fill,grow]",
                        "[fill,grow]"));
        final ChartPanel chartPanel = new ChartPanel(chart);
        this.add(chartPanel, "cell 0 0 1 1");
        this.initialTimestamp = this.getRoundedTimestamp(
                System.currentTimeMillis());
    }

    public void updateTheoSnapshot(final TheoreticalSnapshot snapshot) {
        final long ts = this.getRoundedTimestamp(snapshot.timestamp());
        final Millisecond now = new Millisecond(new Date(ts));
        final PricingChartData chartData = this.dataByInstrument.computeIfAbsent(
                snapshot.securityId(), k -> new PricingChartData());
        chartData.getMmBidSeries().addOrUpdate(now, snapshot.mmBaseBid());
        chartData.getMmAskSeries().addOrUpdate(now, snapshot.mmBaseAsk());
        chartData.getTheoSeries().addOrUpdate(now, snapshot.price());
        if (ts > initialTimestamp + numberOfValues * timeframe) {
            removeDataForTimestamp(chartData);
        }
    }

    public void updateQuote(final Quote quote) {
        final long ts = this.getRoundedTimestamp(quote.timestamp());
        final Millisecond now = new Millisecond(new Date(ts));
        final PricingChartData chartData = this.dataByInstrument.computeIfAbsent(
                quote.securityId(), k -> new PricingChartData());
        chartData.getBidSeries().addOrUpdate(now, Conversion.convertPrice(quote.bid()));
        chartData.getAskSeries().addOrUpdate(now, Conversion.convertPrice(quote.ask()));
        if (ts > initialTimestamp + numberOfValues * timeframe) {
            removeDataForTimestamp(chartData);
        }
    }

    private static JFreeChart createChart() {
        final XYPlot plot = new XYPlot();
        plot.setOrientation(PlotOrientation.VERTICAL);

        XYItemRenderer renderer = new XYLineAndShapeRenderer(
                true, false);
        plot.setRenderer(0, renderer);

        ValueAxis domainAxis = new DateAxis("Time");
        domainAxis.setAutoRange(true);
        ValueAxis priceAxis = new NumberAxis("Price ($)");
        plot.setDomainAxis(0, domainAxis);
        plot.setRangeAxis(0, priceAxis);

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

        return new JFreeChart("Theoretical Snapshot",
                JFreeChart.DEFAULT_TITLE_FONT, plot, true);
    }

    public void setSelectedInstrument(final long instrumentId) {
        this.selectedInstrumentId = instrumentId;
        if (this.dataByInstrument.containsKey(this.selectedInstrumentId)) {
            final XYPlot plot = (XYPlot) this.chart.getPlot();
            plot.setDataset(this.dataByInstrument.get(
                    this.selectedInstrumentId).getDataset());
            plot.mapDatasetToDomainAxis(0, 0);
            plot.mapDatasetToRangeAxis(0, 0);
        }
    }

    private long getRoundedTimestamp(final long timestampInNanos) {
        final long tsInMillis = timestampInNanos / 1000000;
        return (tsInMillis / this.timeframe) * this.timeframe;
    }

    private void removeDataForTimestamp(final PricingChartData chartData) {
        final var removeTS = new Millisecond(new Date(initialTimestamp));
        chartData.getMmBidSeries().delete(removeTS);
        chartData.getMmAskSeries().delete(removeTS);
        chartData.getTheoSeries().delete(removeTS);
        chartData.getBidSeries().delete(removeTS);
        chartData.getAskSeries().delete(removeTS);
        initialTimestamp += timeframe;
    }
}
