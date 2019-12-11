package krypto.ui.components;

import javax.swing.*;
import java.util.List;

public class ListBasedComboboxModel extends DefaultComboBoxModel<String>
{

	private List<String> comboboxItems;

	public ListBasedComboboxModel(List<String> items)
	{
		comboboxItems = items;
	}

	@Override
	public String getElementAt(int index)
	{
		return comboboxItems.get(index);
	}

	@Override
	public void addElement(String item)
	{
		if (!comboboxItems.contains(item))
		{
			comboboxItems.add(item);
		}
	}
}
