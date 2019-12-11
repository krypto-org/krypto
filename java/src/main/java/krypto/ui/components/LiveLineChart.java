package krypto.ui.components;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.ui.ApplicationFrame;
import org.jfree.data.xy.XYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import javax.swing.*;
import java.awt.*;
import java.text.DecimalFormat;
import java.util.Map;

public class LiveLineChart extends ApplicationFrame
{
	private static final long serialVersionUID = 100L;

	private XYSeries series;
	private JFreeChart chart;
	private boolean isYAxisPercentage;

	public LiveLineChart(JPanel panel, String chartTitle, String seriesTitle,
			String xTitle, String yTitle, boolean isYAxisPercentage)
	{
		super(chartTitle);

		this.series = new XYSeries(seriesTitle);
		this.isYAxisPercentage = isYAxisPercentage;

		final XYSeriesCollection dataset = new XYSeriesCollection();
		dataset.addSeries(this.series);
		this.chart = createChart(dataset, chartTitle, xTitle, yTitle);

		final ChartPanel chartPanel = new ChartPanel(chart);
		panel.removeAll();
		panel.add(chartPanel, "cell 0 0");
	}

	public void updateCurve(Map<Double, Double> xyValues)
	{

		this.series.clear();

		for (Map.Entry<Double, Double> xy : xyValues.entrySet())
		{
			this.series.add(xy.getKey(), xy.getValue());
		}
	}

	private JFreeChart createChart(final XYDataset dataset, String chartTitle,
			String xTitle, String yTitle)
	{
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

		if (isYAxisPercentage == true)
		{
			DecimalFormat pctFormat = new DecimalFormat("#.0%");
			((NumberAxis) rangeAxis).setNumberFormatOverride(pctFormat);
		}

		return result;
	}

}
