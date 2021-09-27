// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class Position extends Struct {
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public Position __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public long timestamp() { return bb.getLong(bb_pos + 0); }
  public long securityId() { return bb.getLong(bb_pos + 8); }
  public long position() { return bb.getLong(bb_pos + 16); }
  public double pnl() { return bb.getDouble(bb_pos + 24); }

  public static int createPosition(FlatBufferBuilder builder, long timestamp, long securityId, long position, double pnl) {
    builder.prep(8, 32);
    builder.putDouble(pnl);
    builder.putLong(position);
    builder.putLong(securityId);
    builder.putLong(timestamp);
    return builder.offset();
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public Position get(int j) { return get(new Position(), j); }
    public Position get(Position obj, int j) {  return obj.__assign(__element(j), bb); }
  }
}

