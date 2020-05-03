package krypto.ui.components;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

public class FileChooserActionListener implements ActionListener
{
	private final Component parent;
	private final JTextField textField;
	private String defaultPath = ".";

	public FileChooserActionListener(Component parent, JTextField textField)
	{
		this.parent = parent;
		this.textField = textField;
	}

	@Override
	public void actionPerformed(ActionEvent e)
	{
		JFileChooser fileChooser = new JFileChooser();
		fileChooser.setCurrentDirectory(new File(defaultPath));
		int result = fileChooser.showOpenDialog(parent);

		if (result == JFileChooser.APPROVE_OPTION)
		{
			this.textField.setText(fileChooser.getSelectedFile().getPath());
			this.defaultPath = fileChooser.getSelectedFile().getPath();
		}
	}

	public void setDefaultPath(String defaultPath)
	{
		this.defaultPath = defaultPath;
	}
}
