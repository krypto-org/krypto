package krypto.ui.components;

import java.awt.*;

public class ColorConstants
{
	public static class CustomColors
	{
		public static final Color APPLICATION_BLUE = new Color(46, 76, 92);
		public static final Color APPLICATION_RED = new Color(155, 17, 30);
		public static final Color APPLICATION_GOLDEN = new Color(255, 215, 0);
		public static final Color APPLICATION_DARK_GRAY = new Color(50, 50, 50);
		public static final Color APPLICATION_DARKER_GRAY = new Color(25, 25,
				25);
		public static final Color APPLICATION_TITLE_COLUMN = /*
																 * new Color(25,
																 * 25, 50);
																 */ /* APPLICATION_DARKER_GRAY; */ new Color(
				10, 36, 75);
		public static final Color APPLICATION_TITLE_COLUMN_ALTERNATE = new Color(
				77, 2, 29);
		public static final Color APPLICATION_CHARCOAL = new Color(21, 27, 31);
	}

	public static Color BID_QUANTITY_COLUMN_COLOR = CustomColors.APPLICATION_BLUE;
	public static Color BID_QUANTITY_TEXT_COLOR = Color.WHITE;
	public static Color BID_PRICE_COLUMN_COLOR = CustomColors.APPLICATION_BLUE;
	public static Color BID_PRICE_TEXT_COLOR = Color.WHITE;

	public static Color ASK_QUANTITY_COLUMN_COLOR = CustomColors.APPLICATION_RED;
	public static Color ASK_QUANTITY_TEXT_COLOR = Color.WHITE;
	public static Color ASK_PRICE_COLUMN_COLOR = CustomColors.APPLICATION_RED;
	public static Color ASK_PRICE_TEXT_COLOR = Color.WHITE;

	public static Color UNDERLYING_PRICE_COLUMN_COLOR = CustomColors.APPLICATION_DARKER_GRAY;
	public static Color UNDERLYING_PRICE_TEXT_COLOR = CustomColors.APPLICATION_GOLDEN;

	public static Color VIX_UPDATES_COLUMN_COLOR = CustomColors.APPLICATION_DARKER_GRAY;
	public static Color VIX_UPDATES_TEXT_COLOR = CustomColors.APPLICATION_GOLDEN;

	public static Color POSITION_QUANTITY_POSITIVE_COLOR = CustomColors.APPLICATION_BLUE;
	public static Color POSITION_QUANTITY_NEGATIVE_COLOR = CustomColors.APPLICATION_RED;

	public static Color CALL_COLOR = CustomColors.APPLICATION_TITLE_COLUMN;
	public static Color PUT_COLOR = CustomColors.APPLICATION_TITLE_COLUMN_ALTERNATE;

	public static Color TOTAL_COLOR = CustomColors.APPLICATION_CHARCOAL;

}
