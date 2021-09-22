package krypto.mktdata;

import krypto.network.BaseSubscriber;
import krypto.network.MessageType;
import krypto.serialization.Heartbeat;
import krypto.serialization.OrderUpdate;
import krypto.serialization.Quote;
import krypto.serialization.Trade;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.zeromq.ZMQ;

import java.nio.ByteBuffer;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class MktdataSubscriber extends BaseSubscriber {
    private static final Logger LOGGER = LogManager.getLogger(MktdataSubscriber.class);

    public interface Listener {
        void onHeartbeat(final Heartbeat hb);

        void onQuote(final Quote quote);

        void onTrade(final Trade quote);

        void onOrderUpdate(final OrderUpdate ou);
    }

    private final Set<Listener> listeners = new HashSet<>();

    public MktdataSubscriber(ZMQ.Context context,
                             List<String> addresses,
                             boolean monitor) {
        super(context, addresses, monitor);
    }

    @Override
    protected void process() {
        super.addresses.forEach(x -> LOGGER.info(String.format(
                "Connecting to mktdata gateway @ %s", x)));

        try {
            while (!super.terminated) {
                final String topic = this.socket.recvStr();

                final byte messageType = MessageType.MsgTypeNames.get(topic.substring(0, 2));

                switch (messageType) {

                    case MessageType.HEARTBEAT:
                        this.listeners.forEach(listener -> listener.onHeartbeat(
                                Heartbeat.getRootAsHeartbeat(
                                        ByteBuffer.wrap(socket.recv()))));
                        break;
                    case MessageType.QUOTE:
                        this.listeners.forEach(listener -> listener.onQuote(
                                Quote.getRootAsQuote(
                                        ByteBuffer.wrap(socket.recv()))));
                        break;
                    case MessageType.TRADE:
                        this.listeners.forEach(listener ->
                                listener.onTrade(Trade.getRootAsTrade(
                                        ByteBuffer.wrap(socket.recv()))));
                        break;
                    case MessageType.ORDER_UPDATE:
                        this.listeners.forEach(listener -> listener.onOrderUpdate(
                                OrderUpdate.getRootAsOrderUpdate(
                                        ByteBuffer.wrap(socket.recv()))));
                        break;
                    default:
                        LOGGER.error("Unknown message type received");
                        break;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            super.socket.close();
        }
    }

    public void registerListener(final Listener listener) {
        this.listeners.add(listener);
    }

    public void unregisterListener(final Listener listener) {
        this.listeners.remove(listener);
    }
}
