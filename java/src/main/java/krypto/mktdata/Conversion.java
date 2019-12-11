package krypto.mktdata;

public class Conversion {
    private static double PRICE_MULT = 1000000d;
    private static double QUANTITY_MULT = 10000000000d;

    public static double convertPrice(final long price) {
        return price / PRICE_MULT;
    }

    public static double convertQuantity(final long quantity) {
        return quantity / QUANTITY_MULT;
    }
}
