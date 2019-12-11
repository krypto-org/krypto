package krypto.ui.components;

import javax.swing.*;
import java.awt.*;

public class ImageButton extends JButton
{
	public ImageButton(Image image, int width, int height)
	{
		ImageIcon icon = new ImageIcon(image.getScaledInstance(width, height,
				Image.SCALE_SMOOTH));
		this.setIcon(icon);
		this.setPreferredSize(new Dimension(width, height));
	}
}