package krypto.orders;

import java.util.UUID;

public class Fill {

  private final String fillId;
  private final String orderId;
  private final byte side;
  private final double price;
  private final double filledQty;
  private final double remainingQty;

  public Fill(
      final String orderId,
      final byte side,
      final double price,
      final double filledQty,
      final double remainingQty) {
    this.side = side;
    this.price = price;
    this.fillId = UUID.randomUUID().toString();
    this.orderId = orderId;
    this.filledQty = filledQty;
    this.remainingQty = remainingQty;
  }

  public String getFillId() {
    return fillId;
  }

  public byte getSide() {
    return side;
  }

  public double getPrice() {
    return price;
  }

  public String getOrderId() {
    return orderId;
  }

  public double getFilledQty() {
    return filledQty;
  }

  public double getRemainingQty() {
    return remainingQty;
  }
}
