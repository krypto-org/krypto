package krypto.ui.components;

import javax.swing.*;
import javax.swing.border.LineBorder;
import javax.swing.table.TableCellRenderer;
import java.awt.*;

public class QuadTableCellRenderer extends QuadTableCellPanel
		implements TableCellRenderer
{
	@Override
	public Component getTableCellRendererComponent(JTable table, Object value,
			boolean isSelected, boolean hasFocus, int row, int column)
	{
		setOpaque(true);
		setBackground(isSelected ? Color.BLUE : table.getBackground());
		setBorder(isSelected ? new LineBorder(Color.BLUE, 2)
				: new LineBorder(Color.GRAY, 2));
		return this;
	}
}
