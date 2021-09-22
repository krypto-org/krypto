package krypto.ui.orders;

public enum TimeInForce {
  DAY(krypto.serialization.TimeInForce.DAY),
  IOC(krypto.serialization.TimeInForce.IOC),
  FOK(krypto.serialization.TimeInForce.FOK),
  GTC(krypto.serialization.TimeInForce.GTC),
  GTT(krypto.serialization.TimeInForce.GTT);

  private final byte value;

  TimeInForce(final byte value) {
    this.value = value;
  }

  public byte getValue() {
    return value;
  }
}
