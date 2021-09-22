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
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.data.time.Second;

import javax.swing.*;
import java.awt.*;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

public class PricingChartPanel extends JPanel {

  private static final int MILLIS_IN_SECOND = 1000;

  private final Map<Long, PricingChartData> dataByInstrument = new HashMap<>();
  private final JFreeChart chart = createChart();

  private final int numberOfValues;
  private long firstChartTimestamp;

  public PricingChartPanel(final int numberOfValues) {
    this.numberOfValues = numberOfValues;
    this.setLayout(new MigLayout("", "[fill,grow]", "[fill,grow]"));
    final ChartPanel chartPanel = new ChartPanel(chart);
    this.add(chartPanel, "cell 0 0 1 1");
    this.firstChartTimestamp =
        ((System.currentTimeMillis() / MILLIS_IN_SECOND) * MILLIS_IN_SECOND)
            - (long) numberOfValues * MILLIS_IN_SECOND;
  }

  public void updateTheoSnapshot(final TheoreticalSnapshot snapshot) {
    final long ts = this.getRoundedTimestamp(snapshot.timestamp());
    final Second now = new Second(new Date(ts));
    final PricingChartData chartData =
        this.dataByInstrument.computeIfAbsent(snapshot.securityId(), k -> new PricingChartData());
    chartData.getMmBidSeries().addOrUpdate(now, snapshot.mmBaseBid());
    chartData.getMmAskSeries().addOrUpdate(now, snapshot.mmBaseAsk());
    chartData.getTheoSeries().addOrUpdate(now, snapshot.price());
    this.removeAgedItems(ts);
  }

  public void updateQuote(final Quote quote) {
    final long ts = this.getRoundedTimestamp(quote.timestamp());
    final Second now = new Second(new Date(ts));
    final PricingChartData chartData =
        this.dataByInstrument.computeIfAbsent(quote.securityId(), k -> new PricingChartData());
    chartData.getBidSeries().addOrUpdate(now, Conversion.convertPrice(quote.bid()));
    chartData.getAskSeries().addOrUpdate(now, Conversion.convertPrice(quote.ask()));
    this.removeAgedItems(ts);
  }

  private static JFreeChart createChart() {
    final XYPlot plot = new XYPlot();
    plot.setOrientation(PlotOrientation.VERTICAL);

    XYItemRenderer renderer = new XYLineAndShapeRenderer(true, false);
    plot.setRenderer(0, renderer);

    DateAxis domainAxis = new DateAxis("Time");
    domainAxis.setAutoRange(true);
    domainAxis.setTickLabelPaint(Color.WHITE);
    domainAxis.setLabelPaint(Color.WHITE);

    NumberAxis priceAxis = new NumberAxis("Price ($)");
    priceAxis.setAutoRange(true);
    priceAxis.setAutoRangeIncludesZero(false);
    priceAxis.setTickLabelPaint(Color.WHITE);
    priceAxis.setLabelPaint(Color.WHITE);

    plot.setDomainAxis(0, domainAxis);
    plot.setRangeAxis(0, priceAxis);

    plot.getRenderer().setSeriesPaint(0, ColorConstants.THEO_BID_BACKGROUND_COLOR);
    plot.getRenderer().setSeriesPaint(1, ColorConstants.THEO_ASK_BACKGROUND_COLOR);
    plot.getRenderer().setSeriesPaint(2, ColorConstants.BID_BACKGROUND_COLOR);
    plot.getRenderer().setSeriesPaint(3, ColorConstants.ASK_BACKGROUND_COLOR);
    plot.getRenderer().setSeriesPaint(4, ColorConstants.CustomColors.APPLICATION_GOLDEN);

    plot.setBackgroundPaint(ColorConstants.TOTAL_COLOR);
    plot.setRangeGridlinePaint(Color.WHITE);

    final var result =
        new JFreeChart("Theoretical Snapshot", JFreeChart.DEFAULT_TITLE_FONT, plot, true);

    result.setBackgroundPaint(ColorConstants.TOTAL_COLOR);
    result.getTitle().setBackgroundPaint(ColorConstants.TOTAL_COLOR);
    result.getTitle().setPaint(Color.WHITE);

    result.setBorderPaint(ColorConstants.TOTAL_COLOR);
    result.getLegend().setBackgroundPaint(ColorConstants.TOTAL_COLOR);
    result.getLegend().setItemPaint(Color.WHITE);

    return result;
  }

  public void setSelectedInstrument(final long instrumentId) {
    if (this.dataByInstrument.containsKey(instrumentId)) {
      final XYPlot plot = (XYPlot) this.chart.getPlot();
      plot.setDataset(this.dataByInstrument.get(instrumentId).getDataset());
      plot.mapDatasetToDomainAxis(0, 0);
      plot.mapDatasetToRangeAxis(0, 0);
      plot.getDomainAxis().setAutoRange(true);
    }
  }

  private long getRoundedTimestamp(final long timestampInNanos) {
    final long tsInMillis = timestampInNanos / 1000000;
    return (tsInMillis / MILLIS_IN_SECOND) * MILLIS_IN_SECOND;
  }

  private void removeAgedItems(final long timestamp) {
    while (timestamp - firstChartTimestamp >= this.numberOfValues * MILLIS_IN_SECOND) {
      final Second removeTs = new Second(new Date(firstChartTimestamp));
      dataByInstrument
          .values()
          .forEach(
              chartData -> {
                chartData.getMmBidSeries().delete(removeTs);
                chartData.getMmAskSeries().delete(removeTs);
                chartData.getTheoSeries().delete(removeTs);
                chartData.getBidSeries().delete(removeTs);
                chartData.getAskSeries().delete(removeTs);
              });
      firstChartTimestamp += MILLIS_IN_SECOND;
    }
  }
}
