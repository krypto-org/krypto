// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class Incremental extends Table {
  public static Incremental getRootAsIncremental(ByteBuffer _bb) { return getRootAsIncremental(_bb, new Incremental()); }
  public static Incremental getRootAsIncremental(ByteBuffer _bb, Incremental obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; vtable_start = bb_pos - bb.getInt(bb_pos); vtable_size = bb.getShort(vtable_start); }
  public Incremental __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long timestamp() { int o = __offset(4); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public long securityId() { int o = __offset(6); return o != 0 ? bb.getLong(o + bb_pos) : 0L; }
  public IncrementalPriceLevel updates(int j) { return updates(new IncrementalPriceLevel(), j); }
  public IncrementalPriceLevel updates(IncrementalPriceLevel obj, int j) { int o = __offset(8); return o != 0 ? obj.__assign(__vector(o) + j * 24, bb) : null; }
  public int updatesLength() { int o = __offset(8); return o != 0 ? __vector_len(o) : 0; }

  public static int createIncremental(FlatBufferBuilder builder,
      long timestamp,
      long security_id,
      int updatesOffset) {
    builder.startObject(3);
    Incremental.addSecurityId(builder, security_id);
    Incremental.addTimestamp(builder, timestamp);
    Incremental.addUpdates(builder, updatesOffset);
    return Incremental.endIncremental(builder);
  }

  public static void startIncremental(FlatBufferBuilder builder) { builder.startObject(3); }
  public static void addTimestamp(FlatBufferBuilder builder, long timestamp) { builder.addLong(0, timestamp, 0L); }
  public static void addSecurityId(FlatBufferBuilder builder, long securityId) { builder.addLong(1, securityId, 0L); }
  public static void addUpdates(FlatBufferBuilder builder, int updatesOffset) { builder.addOffset(2, updatesOffset, 0); }
  public static void startUpdatesVector(FlatBufferBuilder builder, int numElems) { builder.startVector(24, numElems, 8); }
  public static int endIncremental(FlatBufferBuilder builder) {
    int o = builder.endObject();
    return o;
  }
}
