package krypto.ui.components;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.ui.ApplicationFrame;
import org.jfree.data.time.Second;
import org.jfree.data.time.TimeSeries;
import org.jfree.data.time.TimeSeriesCollection;
import org.jfree.data.xy.XYDataset;

import javax.swing.*;
import java.awt.*;

public class LiveTimeSeriesChart extends ApplicationFrame {
    private static final long serialVersionUID = 100L;

    private final TimeSeries series;

	public LiveTimeSeriesChart(JPanel panel, String chartTitle,
                               String seriesTitle, String xTitle, String yTitle) {
        super(chartTitle);

        this.series = new TimeSeries(seriesTitle);

        final TimeSeriesCollection dataset = new TimeSeriesCollection(
                this.series);

		JFreeChart chart = createChart(dataset, chartTitle, xTitle, yTitle);

        final ChartPanel chartPanel = new ChartPanel(chart);
        panel.removeAll();
        panel.add(chartPanel, "cell 0 0");
    }

    public void updateCurve(double newValue) {
        final Second now = new Second();
        this.series.add(now, newValue);
    }

    private JFreeChart createChart(final XYDataset dataset, String chartTitle,
                                   String xTitle, String yTitle) {
        final JFreeChart result = ChartFactory.createXYLineChart(chartTitle, // chart
                // title
                xTitle, // x axis label
                yTitle, // y axis label
                dataset, // data
                PlotOrientation.VERTICAL, true, // include legend
                true, // tooltips
                false // urls
        );

        result.setBackgroundPaint(Color.DARK_GRAY);
        result.getTitle().setBackgroundPaint(Color.DARK_GRAY);
        result.getTitle().setPaint(Color.WHITE);
        result.getTitle().setFont(new Font("Cambria Math", Font.BOLD, 14));

        result.setBorderPaint(Color.DARK_GRAY);
        result.getLegend().setBackgroundPaint(Color.DARK_GRAY);
        result.getLegend().setItemPaint(Color.WHITE);
        result.getLegend().setItemFont(new Font("Cambria Math", Font.BOLD, 11));

        final XYPlot plot = result.getXYPlot();
        plot.setBackgroundPaint(Color.DARK_GRAY);
        plot.setRangeGridlinePaint(Color.WHITE);
        plot.getRenderer().setSeriesPaint(0,
                ColorConstants.CustomColors.APPLICATION_GOLDEN);

        ValueAxis valueAxis = plot.getDomainAxis();
        valueAxis.setAutoRange(true);
        valueAxis.setTickLabelPaint(Color.WHITE);
        valueAxis.setTickLabelFont(new Font("Cambria Math", Font.BOLD, 11));
        valueAxis.setLabelFont(new Font("Cambria Math", Font.BOLD, 11));
        valueAxis.setLabelPaint(Color.WHITE);

        ValueAxis rangeAxis = plot.getRangeAxis();
        rangeAxis.setTickLabelPaint(Color.WHITE);
        rangeAxis.setTickLabelFont(new Font("Cambria Math", Font.BOLD, 11));
        rangeAxis.setLabelFont(new Font("Cambria Math", Font.BOLD, 11));
        rangeAxis.setLabelPaint(Color.WHITE);

        return result;
    }

}
