// automatically generated by the FlatBuffers compiler, do not modify

package krypto.serialization;

import java.nio.*;
import java.lang.*;
import java.util.*;
import com.google.flatbuffers.*;

@SuppressWarnings("unused")
public final class BookPosition extends Table {
  public static void ValidateVersion() { Constants.FLATBUFFERS_1_12_0(); }
  public static BookPosition getRootAsBookPosition(ByteBuffer _bb) { return getRootAsBookPosition(_bb, new BookPosition()); }
  public static BookPosition getRootAsBookPosition(ByteBuffer _bb, BookPosition obj) { _bb.order(ByteOrder.LITTLE_ENDIAN); return (obj.__assign(_bb.getInt(_bb.position()) + _bb.position(), _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __reset(_i, _bb); }
  public BookPosition __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public String book() { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; }
  public ByteBuffer bookAsByteBuffer() { return __vector_as_bytebuffer(4, 1); }
  public ByteBuffer bookInByteBuffer(ByteBuffer _bb) { return __vector_in_bytebuffer(_bb, 4, 1); }
  public krypto.serialization.Position positions(int j) { return positions(new krypto.serialization.Position(), j); }
  public krypto.serialization.Position positions(krypto.serialization.Position obj, int j) { int o = __offset(6); return o != 0 ? obj.__assign(__vector(o) + j * 40, bb) : null; }
  public int positionsLength() { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; }
  public krypto.serialization.Position.Vector positionsVector() { return positionsVector(new krypto.serialization.Position.Vector()); }
  public krypto.serialization.Position.Vector positionsVector(krypto.serialization.Position.Vector obj) { int o = __offset(6); return o != 0 ? obj.__assign(__vector(o), 40, bb) : null; }

  public static int createBookPosition(FlatBufferBuilder builder,
      int bookOffset,
      int positionsOffset) {
    builder.startTable(2);
    BookPosition.addPositions(builder, positionsOffset);
    BookPosition.addBook(builder, bookOffset);
    return BookPosition.endBookPosition(builder);
  }

  public static void startBookPosition(FlatBufferBuilder builder) { builder.startTable(2); }
  public static void addBook(FlatBufferBuilder builder, int bookOffset) { builder.addOffset(0, bookOffset, 0); }
  public static void addPositions(FlatBufferBuilder builder, int positionsOffset) { builder.addOffset(1, positionsOffset, 0); }
  public static void startPositionsVector(FlatBufferBuilder builder, int numElems) { builder.startVector(40, numElems, 8); }
  public static int endBookPosition(FlatBufferBuilder builder) {
    int o = builder.endTable();
    return o;
  }

  public static final class Vector extends BaseVector {
    public Vector __assign(int _vector, int _element_size, ByteBuffer _bb) { __reset(_vector, _element_size, _bb); return this; }

    public BookPosition get(int j) { return get(new BookPosition(), j); }
    public BookPosition get(BookPosition obj, int j) {  return obj.__assign(__indirect(__element(j), bb), bb); }
  }
}
