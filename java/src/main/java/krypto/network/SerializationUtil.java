package krypto.network;

import com.google.flatbuffers.FlatBufferBuilder;
import krypto.instruments.Request;
import krypto.serialization.Instrument;
import krypto.serialization.InstrumentRefreshRequest;
import krypto.serialization.InstrumentRequest;

public class SerializationUtil {
    public static byte[] serializeInstrumentRequest(
            final Request request,
            final FlatBufferBuilder bufferBuilder) {
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
}
