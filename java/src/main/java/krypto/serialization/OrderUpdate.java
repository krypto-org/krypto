// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class OrderUpdate extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_1_12_0(); }
  public static OrderUpdate getRootAsOrderUpdate(ByteBuffer _bb) { return getRootAsOrderUpdate(_bb, new OrderUpdate()); }
  public static OrderUpdate getRootAsOrderUpdate(ByteBuffer _bb, OrderUpdate obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public OrderUpdate __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long timestamp() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public String orderId() { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer orderIdAsByteBuffer() { return __vector_as_bytebuffer(6, 1); }
  public ByteBuffer orderIdInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 6, 1); }
  public String exchangeOrderId() { int o = __offset(8); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer exchangeOrderIdAsByteBuffer() { return __vector_as_bytebuffer(8, 1); }
  public ByteBuffer exchangeOrderIdInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 8, 1); }
  public byte status() { int o = __offset(10); return o != 0 ? bb.get(o + bb_pos) : 0; }
  public long filledQuantity() { int o = __offset(12); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long filledPrice() { int o = __offset(14); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }

  public static int createOrderUpdate(FlatBufferBuilder builder,
      long timestamp,
      int order_idOffset,
      int exchange_order_idOffset,
      byte status,
      long filled_quantity,
      long filled_price) {
    builder.startTable(6);
    OrderUpdate.addFilledPrice(builder, filled_price);
    OrderUpdate.addFilledQuantity(builder, filled_quantity);
    OrderUpdate.addTimestamp(builder, timestamp);
    OrderUpdate.addExchangeOrderId(builder, exchange_order_idOffset);
    OrderUpdate.addOrderId(builder, order_idOffset);
    OrderUpdate.addStatus(builder, status);
    return OrderUpdate.endOrderUpdate(builder);
  }

  public static void startOrderUpdate(FlatBufferBuilder builder) { builder.startTable(6); }
  public static void addTimestamp(FlatBufferBuilder builder, long timestamp) { builder.addLong(0, timestamp, 0L); }
  public static void addOrderId(FlatBufferBuilder builder, int orderIdOffset) { builder.addOffset(1, orderIdOffset, 0); }
  public static void addExchangeOrderId(FlatBufferBuilder builder, int exchangeOrderIdOffset) { builder.addOffset(2, exchangeOrderIdOffset, 0); }
  public static void addStatus(FlatBufferBuilder builder, byte status) { builder.addByte(3, status, 0); }
  public static void addFilledQuantity(FlatBufferBuilder builder, long filledQuantity) { builder.addLong(4, filledQuantity, 0L); }
  public static void addFilledPrice(FlatBufferBuilder builder, long filledPrice) { builder.addLong(5, filledPrice, 0L); }
  public static int endOrderUpdate(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public OrderUpdate get(int j) { return get(new OrderUpdate(), j); }
    public OrderUpdate get(OrderUpdate obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

