package krypto.ui.components;

import javax.swing.*;
import javax.swing.table.TableModel;
import javax.swing.table.TableRowSorter;

public class LiveEmptyRowSorter extends TableRowSorter<LiveUpdatedTableModel>
{
	public LiveEmptyRowSorter(LiveUpdatedTableModel model)
	{
		super(model);

		RowFilter<TableModel, Object> rf = null;

		try
		{
			rf = RowFilter.regexFilter("", 0);
		}
		catch (java.util.regex.PatternSyntaxException e)
		{
			return;
		}

		super.setRowFilter(rf);
	}
}