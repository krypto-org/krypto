package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableCellRenderer;
import java.awt.*;

public class TableColumnHeaderRenderer extends JLabel implements TableCellRenderer
{
	@Override
	public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	{
		setFont(FontConstants.TABLE_HEADER_FONT);
		setText(value.toString());
		setHorizontalAlignment(SwingConstants.CENTER);
		setVerticalAlignment(SwingConstants.CENTER);
		return this;
	}

}
