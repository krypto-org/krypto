package krypto.ui.components;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.chart.renderer.xy.XYItemRenderer;
import org.jfree.chart.renderer.xy.XYLineAndShapeRenderer;
import org.jfree.chart.ui.ApplicationFrame;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

import javax.swing.*;
import java.awt.*;
import java.util.HashMap;
import java.util.Map;

public class ModelFitChart extends ApplicationFrame
{
	private XYSeries marketSeries;
	private XYSeries fittedSeries;
	private XYSeriesCollection marketDataset;
	private XYSeriesCollection fittedDataset;
	private JFreeChart chart;
	private Map<Double, Double> marketData = new HashMap<Double, Double>();
	private Map<Double, Double> fittedData = new HashMap<Double, Double>();

	public ModelFitChart(JPanel panel, String chartTitle, String scatterSeriesTitle,
			String lineSeriesTitle,
			String xLabel, String yLabel)
	{
		super(chartTitle);
		this.marketSeries = new XYSeries(scatterSeriesTitle);
		this.fittedSeries = new XYSeries(lineSeriesTitle);

		this.marketDataset = new XYSeriesCollection();
		this.fittedDataset = new XYSeriesCollection();
		this.marketDataset.addSeries(this.marketSeries);
		this.fittedDataset.addSeries(this.fittedSeries);
		this.chart = createChart(chartTitle, xLabel, yLabel);

		final ChartPanel chartPanel = new ChartPanel(chart);
		panel.removeAll();
		panel.add(chartPanel, "cell 0 0");
	}

	public void updateMarketCurve(Map<Double, Double> xyValues)
	{
		this.marketData = xyValues;

		this.marketSeries.clear();
		this.fittedData.clear();

		for (Map.Entry<Double, Double> xy : xyValues.entrySet())
		{
			this.marketSeries.add(xy.getKey(), xy.getValue());
		}
	}

	public void updateFittedCurve(Map<Double, Double> xyValues)
	{
		this.fittedData = xyValues;

		this.fittedSeries.clear();
		this.marketData.clear();

		for (Map.Entry<Double, Double> xy : fittedData.entrySet())
		{
			this.fittedSeries.add(xy.getKey(), xy.getValue());
		}

		for (Map.Entry<Double, Double> xy : marketData.entrySet())
		{
			this.fittedSeries.add(xy.getKey(), xy.getValue());
		}
	}

	private JFreeChart createChart(String chartTitle, String xTitle,
			String yTitle)
	{
		final JFreeChart result = ChartFactory.createScatterPlot(chartTitle,
				xTitle, yTitle, marketDataset, PlotOrientation.VERTICAL, true,
				true, false);

		final XYPlot plot = result.getXYPlot();
		plot.setDataset(0, marketDataset);
		plot.setBackgroundPaint(Color.DARK_GRAY);
		plot.setRangeGridlinePaint(Color.WHITE);
		plot.getRenderer().setSeriesPaint(0,
				ColorConstants.CustomColors.APPLICATION_GOLDEN);

		XYItemRenderer lineRenderer = new XYLineAndShapeRenderer(true, false);

		// Set the line data, renderer, and axis into plot
		plot.setDataset(1, fittedDataset);
		plot.setRenderer(1, lineRenderer);

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

		result.setBackgroundPaint(Color.DARK_GRAY);
		result.getTitle().setBackgroundPaint(Color.DARK_GRAY);
		result.getTitle().setPaint(Color.WHITE);
		result.getTitle().setFont(new Font("Cambria Math", Font.BOLD, 14));

		result.setBorderPaint(Color.DARK_GRAY);
		result.getLegend().setBackgroundPaint(Color.DARK_GRAY);
		result.getLegend().setItemPaint(Color.WHITE);
		result.getLegend().setItemFont(new Font("Cambria Math", Font.BOLD, 11));

		return result;
	}
}
