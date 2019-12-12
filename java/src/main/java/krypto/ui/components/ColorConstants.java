package krypto.ui.components;

import java.awt.*;

public class ColorConstants
{
	public static class CustomColors
	{
//		$color-primary-0: #A23645;	// Main Primary color */
//		$color-primary-1: #F3A2AD;
//		$color-primary-2: #CA6573;
//		$color-primary-3: #791422;
//		$color-primary-4: #51000B;
//
//		$color-secondary-1-0: #AA8F39;	// Main Secondary color (1) */
//		$color-secondary-1-1: #FFEBAA;
//		$color-secondary-1-2: #D4BB6A;
//		$color-secondary-1-3: #806615;
//		$color-secondary-1-4: #554100;
//
//		$color-secondary-2-0: #363377;	// Main Secondary color (2) */
//		$color-secondary-2-1: #817FB2;
//		$color-secondary-2-2: #575594;
//		$color-secondary-2-3: #1C1A59;
//		$color-secondary-2-4: #0A093B;
//
//		$color-complement-0: #479030;	// Main Complement color */
//		$color-complement-1: #A2D890;
//		$color-complement-2: #70B45A;
//		$color-complement-3: #286C12;
//		$color-complement-4: #114800;

		public static final Color APPLICATION_BLUE = Color.decode("#1C1A59");
		public static final Color APPLICATION_RED = Color.decode("#51000B");
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
