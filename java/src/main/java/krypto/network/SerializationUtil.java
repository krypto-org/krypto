package krypto.network;

import com.google.flatbuffers.FlatBufferBuilder;
import krypto.instruments.Request;
import krypto.mktdata.Conversion;
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
            final FlatBufferBuilder builder,
            final String orderId,
            final long securityId,
            final double price,
            final double size,
            final byte side,
            final byte timeInForce) {
        builder.clear();
        int orderIdOffset = builder.createString(orderId);
        OrderRequest.startOrderRequest(builder);
        OrderRequest.addSecurityId(builder, securityId);
        OrderRequest.addPrice(builder, Conversion.extractPrice(price));
        OrderRequest.addQuantity(builder, Conversion.extractQuantity(size));
        OrderRequest.addSide(builder, side);
        OrderRequest.addTif(builder, timeInForce);
        OrderRequest.addOrderId(builder, orderIdOffset);
        int offset = OrderRequest.endOrderRequest(builder);
        builder.finish(offset);
        return builder.sizedByteArray();
    }

    public static byte[] serializeOrderCancelRequest(
            final FlatBufferBuilder builder, final String orderId) {
        builder.clear();
        int orderIdOffset = builder.createString(orderId);
        OrderCancelRequest.startOrderCancelRequest(builder);
        OrderCancelRequest.addOrderId(builder, orderIdOffset);
        OrderCancelRequest.addTimestamp(builder, System.currentTimeMillis() * 1000000L);
        int offset = OrderCancelRequest.endOrderCancelRequest(builder);
        builder.finish(offset);
        return builder.sizedByteArray();
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
