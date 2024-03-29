// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class InstrumentRequest extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_1_12_0(); }
  public static InstrumentRequest getRootAsInstrumentRequest(ByteBuffer _bb) { return getRootAsInstrumentRequest(_bb, new InstrumentRequest()); }
  public static InstrumentRequest getRootAsInstrumentRequest(ByteBuffer _bb, InstrumentRequest obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public InstrumentRequest __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public byte type() { int o = __offset(4); return o != 0 ? bb.get(o + bb_pos) : 0; }

  public static int createInstrumentRequest(FlatBufferBuilder builder,
      byte type) {
    builder.startTable(1);
    InstrumentRequest.addType(builder, type);
    return InstrumentRequest.endInstrumentRequest(builder);
  }

  public static void startInstrumentRequest(FlatBufferBuilder builder) { builder.startTable(1); }
  public static void addType(FlatBufferBuilder builder, byte type) { builder.addByte(0, type, 0); }
  public static int endInstrumentRequest(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public InstrumentRequest get(int j) { return get(new InstrumentRequest(), j); }
    public InstrumentRequest get(InstrumentRequest obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}

