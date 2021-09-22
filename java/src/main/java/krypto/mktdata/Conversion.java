package krypto.mktdata;

public class Conversion {
  private static final double PRICE_MULT = 1000000d;
  private static final double QUANTITY_MULT = 10000000000d;

  public static double convertPrice(final long price) {
    return price / PRICE_MULT;
  }

  public static double convertQuantity(final long quantity) {
    return quantity / QUANTITY_MULT;
  }

  public static long extractPrice(final double price) {
    return (long) (price * PRICE_MULT);
  }

  public static long extractQuantity(final double quantity) {
    return (long) (quantity * QUANTITY_MULT);
  }
}
