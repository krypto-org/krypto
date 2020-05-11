package krypto.orders;

import krypto.serialization.OrderUpdate;

public class Order {
    private final String clOrderId;
    private final long securityId;
    private final String symbol;
    private final double price;
    private final double size;
    private final byte side;
    private final byte tif;

    private String orderId;
    private long lastUpdatedTimestamp;
    private byte status;
    private double filledQty;
    private double fees;

    public Order(
            final String clOrderId,
            final long securityId,
            final String symbol,
            final double price,
            final double size,
            final byte side,
            final byte tif) {
        this.clOrderId = clOrderId;
        this.securityId = securityId;
        this.symbol = symbol;
        this.price = price;
        this.size = size;
        this.side = side;
        this.tif = tif;
    }

    public void setOrderId(final String orderId) {
        this.orderId = orderId;
        this.lastUpdatedTimestamp = System.currentTimeMillis();
    }

    public void setStatus(final byte status) {
        this.status = status;
        this.lastUpdatedTimestamp = System.currentTimeMillis();
    }

    public void setFilledQty(final double filledQty) {
        this.filledQty = filledQty;
        this.lastUpdatedTimestamp = System.currentTimeMillis();
    }

    public void setFees(final double fees) {
        this.fees = fees;
        this.lastUpdatedTimestamp = System.currentTimeMillis();
    }

    public String getClOrderId() {
        return clOrderId;
    }

    public long getSecurityId() {
        return securityId;
    }

    public String getSymbol() {
        return symbol;
    }

    public double getPrice() {
        return price;
    }

    public double getSize() {
        return size;
    }

    public byte getSide() {
        return side;
    }

    public byte getTif() {
        return tif;
    }

    public String getOrderId() {
        return orderId;
    }

    public long getLastUpdatedTimestamp() {
        return lastUpdatedTimestamp;
    }

    public byte getStatus() {
        return status;
    }

    public double getFilledQty() {
        return filledQty;
    }

    public double getFees() {
        return fees;
    }
}
