package krypto.ui.orders;

public enum Side {
  BUY(krypto.serialization.Side.BUY),
  SELL(krypto.serialization.Side.SELL);

  private final byte value;

  Side(final byte value) {
    this.value = value;
  }

  public byte getValue() {
    return value;
  }
}
