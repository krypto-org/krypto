package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.DefaultTableCellRenderer;
import java.awt.*;

public class TotalTableTableCellRenderer extends DefaultTableCellRenderer
{
	private boolean centered = true;

	public TotalTableTableCellRenderer(boolean centered)
	{
		super();
		this.centered = centered;
	}

	public TotalTableTableCellRenderer()
	{
		super();
	}

	@Override
	public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	{

		Component renderer = super.getTableCellRendererComponent(table, value,
				isSelected, hasFocus, row, column);

		if (centered)
		{
			((DefaultTableCellRenderer) renderer)
					.setHorizontalAlignment(SwingConstants.CENTER);
		}

		renderer.setBackground(ColorConstants.TOTAL_COLOR);

		return renderer;
	}
}
