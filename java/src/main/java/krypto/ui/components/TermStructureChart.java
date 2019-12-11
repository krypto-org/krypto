package krypto.ui.components;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.ui.ApplicationFrame;
import org.jfree.data.time.Day;
import org.jfree.data.time.TimeSeries;
import org.jfree.data.time.TimeSeriesCollection;
import org.jfree.data.xy.XYDataset;

import javax.swing.*;
import java.awt.*;
import java.text.DecimalFormat;
import java.util.Calendar;
import java.util.Map;

public class TermStructureChart extends ApplicationFrame
{
	private TimeSeries series;
	private JFreeChart chart;

	public TermStructureChart(JPanel panel, String chartTitle,
			String seriesTitle, String xTitle, String yTitle)
	{
		super(chartTitle);

		this.series = new TimeSeries(seriesTitle);

		final TimeSeriesCollection dataset = new TimeSeriesCollection(
				this.series);

		this.chart = createChart(dataset, chartTitle, xTitle, yTitle);

		final ChartPanel chartPanel = new ChartPanel(chart);
		panel.removeAll();
		panel.add(chartPanel, "cell 0 0");
	}

	public void updateCurve(Map<Calendar, Double> values)
	{
		this.series.clear();
		for (Map.Entry<Calendar, Double> es : values.entrySet())
		{
			this.series.add(new Day(es.getKey().get(Calendar.DAY_OF_MONTH),
					es.getKey().get(Calendar.MONTH) + 1,
					es.getKey().get(Calendar.YEAR)), es.getValue());
		}
	}

	private JFreeChart createChart(final XYDataset dataset, String chartTitle,
			String xTitle, String yTitle)
	{

		final JFreeChart result = ChartFactory.createTimeSeriesChart(chartTitle,
				xTitle, yTitle, dataset, false, false, false);

		result.setBackgroundPaint(Color.DARK_GRAY);
		result.getTitle().setBackgroundPaint(Color.DARK_GRAY);
		result.getTitle().setPaint(Color.WHITE);
		result.getTitle().setFont(new Font("Cambria Math", Font.BOLD, 14));

		result.setBorderPaint(Color.DARK_GRAY);

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

		DecimalFormat pctFormat = new DecimalFormat("#.0%");
		((NumberAxis) rangeAxis).setNumberFormatOverride(pctFormat);
		
		return result;
	}

}
