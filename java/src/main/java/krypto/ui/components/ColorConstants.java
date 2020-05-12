package krypto.ui.components;

import java.awt.*;

public class ColorConstants {
    public static class CustomColors {

/*
* *** Primary color:

   shade 0 = #5C5A87 = rgb( 92, 90,135) = rgba( 92, 90,135,1) = rgb0(0.361,0.353,0.529)
   shade 1 = #ACABC2 = rgb(172,171,194) = rgba(172,171,194,1) = rgb0(0.675,0.671,0.761)
   shade 2 = #7F7EA1 = rgb(127,126,161) = rgba(127,126,161,1) = rgb0(0.498,0.494,0.631)
   shade 3 = #424071 = rgb( 66, 64,113) = rgba( 66, 64,113,1) = rgb0(0.259,0.251,0.443)
   shade 4 = #2B295C = rgb( 43, 41, 92) = rgba( 43, 41, 92,1) = rgb0(0.169,0.161,0.361)

*** Secondary color (1):

   shade 0 = #8BAD6A = rgb(139,173,106) = rgba(139,173,106,1) = rgb0(0.545,0.678,0.416)
   shade 1 = #DAEACA = rgb(218,234,202) = rgba(218,234,202,1) = rgb0(0.855,0.918,0.792)
   shade 2 = #B4CF9A = rgb(180,207,154) = rgba(180,207,154,1) = rgb0(0.706,0.812,0.604)
   shade 3 = #6B9147 = rgb(107,145, 71) = rgba(107,145, 71,1) = rgb0(0.42,0.569,0.278)
   shade 4 = #4F762A = rgb( 79,118, 42) = rgba( 79,118, 42,1) = rgb0(0.31,0.463,0.165)

*** Secondary color (2):

   shade 0 = #B36E7D = rgb(179,110,125) = rgba(179,110,125,1) = rgb0(0.702,0.431,0.49)
   shade 1 = #F0CFD6 = rgb(240,207,214) = rgba(240,207,214,1) = rgb0(0.941,0.812,0.839)
   shade 2 = #D59EAA = rgb(213,158,170) = rgba(213,158,170,1) = rgb0(0.835,0.62,0.667)
   shade 3 = #964A5A = rgb(150, 74, 90) = rgba(150, 74, 90,1) = rgb0(0.588,0.29,0.353)
   shade 4 = #7A2B3C = rgb(122, 43, 60) = rgba(122, 43, 60,1) = rgb0(0.478,0.169,0.235)
* */

        public static final Color DARK_BLUE = new Color(43, 41, 92);
        public static final Color DARK_RED = new Color(122, 43, 60);

        public static final Color MEDIUM_BLUE = new Color(92, 90, 135);
        public static final Color MEDIUM_RED = new Color(150, 74, 90);

        public static final Color LIGHT_BLUE = new Color(127, 126, 161);
        public static final Color LIGHT_RED = new Color(213, 158, 170);

        public static final Color DARK_GREEN = new Color(107,145, 71);
        public static final Color LIGHT_GREEN = new Color(218,234,202);

        public static final Color APPLICATION_GOLDEN = new Color(255, 215, 0);

        public static final Color APPLICATION_CHARCOAL = new Color(21, 27, 31);

    }

    public static Color BID_BACKGROUND_COLOR = CustomColors.DARK_BLUE;
    public static Color BID_FOREGRPUND_COLOR = Color.WHITE;

    public static Color ASK_BACKGROUND_COLOR = CustomColors.DARK_RED;
    public static Color ASK_FOREGRPUND_COLOR = Color.WHITE;

    public static Color THEO_BID_BACKGROUND_COLOR = CustomColors.MEDIUM_BLUE;
    public static Color THEO_BID_FOREGRPUND_COLOR = Color.WHITE;

    public static Color THEO_ASK_BACKGROUND_COLOR = CustomColors.MEDIUM_RED;
    public static Color THEO_ASK_FOREGRPUND_COLOR = Color.WHITE;

    public static Color ACTIVE_ORDER_COLOR = CustomColors.LIGHT_GREEN;

    public static Color CALL_COLOR = CustomColors.LIGHT_BLUE;
    public static Color PUT_COLOR = CustomColors.LIGHT_BLUE;

    public static Color TOTAL_COLOR = CustomColors.APPLICATION_CHARCOAL;
}
