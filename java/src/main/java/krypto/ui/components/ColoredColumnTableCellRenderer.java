package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;

public class ColoredColumnTableCellRenderer extends DefaultTableCellRenderer
{
	private Color backgroundColor;
	private Color foregroundColor;
	private int column;
	private boolean alignCenter;

	public ColoredColumnTableCellRenderer(Color backgroundColor,
			Color foregroundColor, int column, boolean alignCenter)
	{
		super();
		this.backgroundColor = backgroundColor;
		this.foregroundColor = foregroundColor;
		this.column = column;
		this.alignCenter = alignCenter;
	}

	@Override
	public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	{
		Component renderer = super.getTableCellRendererComponent(table, value,
				isSelected, hasFocus, row, column);
		if (column == this.column)
		{
			if (!isSelected)
			{
				renderer.setBackground(this.backgroundColor);
				if (renderer.getBackground() == Color.BLACK)
				{
					renderer.setForeground(this.foregroundColor);
				}
			}
			
			if (this.alignCenter)
			{
				((DefaultTableCellRenderer) renderer)
						.setHorizontalAlignment(SwingConstants.CENTER);
			}

		}
		return renderer;
	}
}
