// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class Incremental extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_1_12_0(); }
  public static Incremental getRootAsIncremental(ByteBuffer _bb) { return getRootAsIncremental(_bb, new Incremental()); }
  public static Incremental getRootAsIncremental(ByteBuffer _bb, Incremental obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public Incremental __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long timestamp() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long securityId() { int o = __offset(6); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public krypto.serialization.IncrementalPriceLevel updates(int j) { return updates(new krypto.serialization.IncrementalPriceLevel(), j); }
  public krypto.serialization.IncrementalPriceLevel updates(krypto.serialization.IncrementalPriceLevel obj, int j) { int o = __offset(8); return o != 0 ? obj.__assign(__vector(o) + j * 24, bb) : null; }
  public int updatesLength() { int o = __offset(8); return o != 0 ? __vector_len(o) : 0; }
  public krypto.serialization.IncrementalPriceLevel.Vector updatesVector() { return updatesVector(new krypto.serialization.IncrementalPriceLevel.Vector()); }
  public krypto.serialization.IncrementalPriceLevel.Vector updatesVector(krypto.serialization.IncrementalPriceLevel.Vector obj) { int o = __offset(8); return o != 0 ? obj.__assign(__vector(o), 24, bb) : null; }

  public static int createIncremental(FlatBufferBuilder builder,
      long timestamp,
      long security_id,
      int updatesOffset) {
    builder.startTable(3);
    Incremental.addSecurityId(builder, security_id);
    Incremental.addTimestamp(builder, timestamp);
    Incremental.addUpdates(builder, updatesOffset);
    return Incremental.endIncremental(builder);
  }

  public static void startIncremental(FlatBufferBuilder builder) { builder.startTable(3); }
  public static void addTimestamp(FlatBufferBuilder builder, long timestamp) { builder.addLong(0, timestamp, 0L); }
  public static void addSecurityId(FlatBufferBuilder builder, long securityId) { builder.addLong(1, securityId, 0L); }
  public static void addUpdates(FlatBufferBuilder builder, int updatesOffset) { builder.addOffset(2, updatesOffset, 0); }
  public static void startUpdatesVector(FlatBufferBuilder builder, int numElems) { builder.startVector(24, numElems, 8); }
  public static int endIncremental(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public Incremental get(int j) { return get(new Incremental(), j); }
    public Incremental get(Incremental obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

