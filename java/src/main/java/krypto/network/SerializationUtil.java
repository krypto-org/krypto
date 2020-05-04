package krypto.network;

import com.google.flatbuffers.FlatBufferBuilder;
import krypto.instruments.Request;
import krypto.serialization.*;

public class SerializationUtil {
    public static byte[] serializeInstrumentRequest(
            final Request request, final FlatBufferBuilder bufferBuilder) {
        bufferBuilder.clear();
        InstrumentRequest.startInstrumentRequest(bufferBuilder);
        InstrumentRequest.addType(bufferBuilder, request.getRequestType());
        int offset = InstrumentRequest.endInstrumentRequest(bufferBuilder);
        bufferBuilder.finish(offset);
        return bufferBuilder.sizedByteArray();
    }

    public static byte[] serializeInstrumentRequestRequest(final FlatBufferBuilder bufferBuilder) {
        bufferBuilder.clear();
        InstrumentRefreshRequest.startInstrumentRefreshRequest(bufferBuilder);
        int offset = InstrumentRefreshRequest.endInstrumentRefreshRequest(bufferBuilder);
        bufferBuilder.finish(offset);
        return bufferBuilder.sizedByteArray();
    }

    public static byte[] serializeOrderRequest(
            final FlatBufferBuilder bufferBuilder,
            final long securityId,
            final double price,
            final int size,
            final Side side,
            final TimeInForce timeInForce) {
        bufferBuilder.clear();
        return bufferBuilder.sizedByteArray();
    }

    public static byte[] serializeOrderCancelRequest(
            final FlatBufferBuilder bufferBuilder, final String orderId) {
        bufferBuilder.clear();
        return bufferBuilder.sizedByteArray();
    }

    public static byte[] serializeOrderReplaceRequest(
            final FlatBufferBuilder bufferBuilder,
            final String orderId,
            final long securityId,
            final double price,
            final int size,
            final Side side,
            final TimeInForce timeInForce) {
        bufferBuilder.clear();
        return bufferBuilder.sizedByteArray();
    }
}
