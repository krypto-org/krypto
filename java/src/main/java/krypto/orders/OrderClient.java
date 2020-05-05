package krypto.orders;

import com.google.flatbuffers.FlatBufferBuilder;
import krypto.network.MessageType;
import krypto.network.SerializationUtil;
import krypto.network.SocketUtils;
import krypto.serialization.OrderUpdate;
import krypto.serialization.Side;
import krypto.serialization.TimeInForce;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.zeromq.SocketType;
import org.zeromq.ZMQ;

import java.nio.ByteBuffer;
import java.util.HashSet;
import java.util.Set;

public class OrderClient {
    private static final Logger logger = LogManager.getLogger(OrderClient.class);
    private static final int ORDER_ID_SIZE = 10;
    private static final String queueEndpoint = "inproc://orders_queue";

    private final Set<OrderListener> listeners = new HashSet<>();
    private final FlatBufferBuilder fb = new FlatBufferBuilder();
    private final OrderIdGenerator orderIdGenerator = new OrderIdGenerator(ORDER_ID_SIZE);
    private final ZMQ.Context context;
    private final ZMQ.Socket sender;
    private final ZMQ.Socket queuePush;
    private final ZMQ.Socket queuePull;
    private final String orderGatewayEndpoint;
    private final int timeoutInMillis;

    private final Thread socketThread;

    private volatile boolean running = false;

    public OrderClient(
            final ZMQ.Context context,
            final String orderGatewayEndpoint,
            final int timeoutInMillis) {
        this.context = context;
        this.sender = context.socket(SocketType.DEALER);
        this.queuePush = context.socket(SocketType.PUSH);
        this.queuePull = context.socket(SocketType.PULL);
        this.orderGatewayEndpoint = orderGatewayEndpoint;
        this.timeoutInMillis = timeoutInMillis;
        this.socketThread = new Thread(this::pollSockets);
    }

    public void start() {
        if (!this.running) {
            queuePush.setLinger(0);
            queuePush.connect(queueEndpoint);
            this.running = true;
            this.socketThread.start();
        }
    }

    public void stop() {
        if (this.running) {
            this.running = false;
            try {
                this.socketThread.join();
            } catch (final InterruptedException e) {
                logger.error("Socket joining failed with error: {}", e.getMessage());
            }
        }
    }

    private void pollSockets() {
        final var identity = SocketUtils.generateIdentity("orders");
        SocketUtils.connect(sender, orderGatewayEndpoint, identity);
        queuePull.bind(queueEndpoint);
        queuePull.setLinger(0);

//        queuePull.setReceiveTimeOut(100);
//        sender.setReceiveTimeOut(100);

        final ZMQ.Poller poller = context.poller(2);
        poller.register(queuePull, ZMQ.Poller.POLLIN);
        poller.register(sender, ZMQ.Poller.POLLIN | ZMQ.Poller.POLLOUT);
        poller.poll(1000);
        while (running) {
            logger.info("Poll 0");
            if (poller.pollin(0)) {
                logger.info("Waiting for message .. ");
                final var exchange = queuePull.recvStr();
                logger.info("received {}", exchange);
                final var messageType = queuePull.recv()[0];
                logger.info("received {}", messageType);
                final var payload = queuePull.recv();
                logger.info("received payload {}", payload.length);
                SocketUtils.sendEmptyFrame(sender, ZMQ.SNDMORE);
                sender.sendMore(exchange);
                SocketUtils.sendMessageType(sender, messageType, ZMQ.SNDMORE);
                sender.send(payload);
            }
            logger.info("Poll 1");
            if (poller.pollin(1)) {
                logger.info("Received something...");
                final var empty = sender.recvStr();
                final var exchange = sender.recvStr();
                final var messageType = SocketUtils.receiveMessageType(sender);

                if (messageType == MessageType.UNDEFINED) {
                    logger.warn("Received undefine message type");
                    continue;
                }

                if (messageType == MessageType.NO_PAYLOAD) {
                    logger.warn("Received message with message type = NO_PAYLOAD");
                    continue;
                }

                final var orderUpdate =
                        OrderUpdate.getRootAsOrderUpdate(
                                ByteBuffer.wrap(sender.recv(0)));

                this.listeners.forEach(listener -> listener.handleOrderEvent(orderUpdate));
            }
        }
    }

    public String newOrder(
            final String exchange,
            final long securityId,
            final double price,
            final double size,
            final byte side,
            final byte timeInForce) {
        final var orderId = orderIdGenerator.generate();
        queuePush.sendMore(exchange);
        SocketUtils.sendMessageType(queuePush, MessageType.ORDER_REQUEST, ZMQ.SNDMORE);
        queuePush.send(
                SerializationUtil.serializeOrderRequest(
                        fb, orderId, securityId, price, size, side, timeInForce));
        return orderId;
    }

    public void cancelOrder(final String exchange, final String orderId) {
        queuePush.send(exchange, ZMQ.SNDMORE);
        SocketUtils.sendMessageType(queuePush, MessageType.ORDER_CANCEL_REQUEST, ZMQ.SNDMORE);
        queuePush.send(SerializationUtil.serializeOrderCancelRequest(fb, orderId));
    }

    public void replaceOrder(
            final String exchange,
            final String orderId,
            final long securityId,
            final double price,
            final int size,
            final Side side,
            final TimeInForce timeInForce) {
        queuePush.send(exchange, ZMQ.SNDMORE);
        SocketUtils.sendMessageType(queuePush, MessageType.ORDER_REPLACE_REQUEST, ZMQ.SNDMORE);
        queuePush.send(SerializationUtil.serializeOrderCancelRequest(fb, orderId));
    }

    public void registerListener(final OrderListener listener) {
        this.listeners.add(listener);
    }

    public void unregisterListener(final OrderListener listener) {
        this.listeners.remove(listener);
    }
}
