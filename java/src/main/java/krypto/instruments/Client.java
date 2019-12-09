package krypto.instruments;


import krypto.network.MessageType;
import krypto.network.SerializationUtil;
import krypto.network.ZmqUtils;
import krypto.serialization.Exchange;
import krypto.serialization.Instrument;
import krypto.serialization.InstrumentResponse;
import krypto.serialization.RequestType;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import com.google.flatbuffers.FlatBufferBuilder;
import org.zeromq.SocketType;
import org.zeromq.ZMQ;

import java.nio.ByteBuffer;
import java.util.SortedMap;
import java.util.TreeMap;

public class Client {
    static {
        System.setProperty("logFilename", "krypto.instrument.client.test.log");
    }

    private static Logger logger = LogManager.getLogger(Client.class);

    private final FlatBufferBuilder fb = new FlatBufferBuilder();
    private final ZMQ.Socket socket;

    public Client(final ZMQ.Context context, final String address) {
        this.socket = context.socket(SocketType.DEALER);
        this.socket.connect(address);
        logger.info("Connected to {}", address);
    }

    public SortedMap<Long, Instrument> getInstruments() {
        return this.getInstruments(RequestType.ALL);
    }

    public SortedMap<Long, Instrument> getInstruments(final byte requestType) {
        logger.info("Sending request for request type {}", RequestType.name(requestType));

        ZmqUtils.sendEmptyFrame(socket, ZMQ.SNDMORE);
        ZmqUtils.sendMessageType(socket, MessageType.INSTRUMENT_REQUEST,
                ZMQ.SNDMORE);
        socket.send(SerializationUtil.serializeInstrumentRequest(new Request(requestType), fb));

        final String empty = socket.recvStr();
        assert empty.equals("");
        final InstrumentResponse response =
                InstrumentResponse.getRootAsInstrumentResponse(ByteBuffer.wrap(this.socket.recv()));
        final SortedMap<Long, Instrument> instruments = new TreeMap<>();
        for (int i = 0; i < response.instrumentsLength(); ++i) {
            final Instrument instrument = response.instruments(i);
            instruments.put(instrument.id(), instrument);
        }
        return instruments;
    }

    public void refreshCache() {
        ZmqUtils.sendEmptyFrame(socket, ZMQ.SNDMORE);
        ZmqUtils.sendMessageType(socket, MessageType.INSTRUMENT_REQUEST,
                ZMQ.SNDMORE);
        socket.send(SerializationUtil.serializeInstrumentRequestRequest(fb));
    }

    public static void main(String[] args) {
        final ZMQ.Context context = ZMQ.context(1);
        final Client client = new Client(context, "tcp://192.168.1.12:12000");
        client.getInstruments().forEach((id, inst) -> {
            logger.info("{}: {}@{}", id, inst.symbol(), Exchange.name( inst.exchange()));
        });
    }
}
