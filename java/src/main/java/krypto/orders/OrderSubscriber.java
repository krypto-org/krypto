package krypto.orders;

import krypto.network.BaseSubscriber;
import krypto.network.MessageType;
import krypto.network.SocketUtils;
import krypto.serialization.OrderCancelRequest;
import krypto.serialization.OrderReplaceRequest;
import krypto.serialization.OrderRequest;
import krypto.serialization.OrderUpdate;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.zeromq.ZMQ;

import java.nio.ByteBuffer;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class OrderSubscriber extends BaseSubscriber {
  private static final Logger LOGGER = LogManager.getLogger(OrderSubscriber.class);

  public interface Listener {
    void onNewOrder(final String client, final OrderRequest orderRequest);

    void onOrderCancel(final String client, final OrderCancelRequest orderCancelRequest);

    void onOrderReplace(final String client, final OrderReplaceRequest orderReplaceRequest);

    void onOrderUpdate(final String client, final OrderUpdate ou);
  }

  private final Set<Listener> listeners = new HashSet<>();

  public OrderSubscriber(ZMQ.Context context, List<String> addresses) {
    super(context, addresses, false);
  }

  @Override
  protected void process() {
    super.addresses.forEach(
        x -> LOGGER.info(String.format("Connecting to mktdata gateway @ %s", x)));
    while (!terminated) {
      final String client = this.socket.recvStr();
      final byte messageType = SocketUtils.receiveMessageType(this.socket);
      final byte[] payload = this.socket.recv();

      switch (messageType) {
        case MessageType.ORDER_REQUEST:
          this.listeners.forEach(
              listener ->
                  listener.onNewOrder(
                      client, OrderRequest.getRootAsOrderRequest(ByteBuffer.wrap(payload))));
          break;
        case MessageType.ORDER_CANCEL_REQUEST:
          this.listeners.forEach(
              listener ->
                  listener.onOrderCancel(
                      client,
                      OrderCancelRequest.getRootAsOrderCancelRequest(ByteBuffer.wrap(payload))));
          break;
        case MessageType.ORDER_REPLACE_REQUEST:
          this.listeners.forEach(
              listener ->
                  listener.onOrderReplace(
                      client,
                      OrderReplaceRequest.getRootAsOrderReplaceRequest(ByteBuffer.wrap(payload))));
          break;
        case MessageType.ORDER_UPDATE:
          this.listeners.forEach(
              listener ->
                  listener.onOrderUpdate(
                      client, OrderUpdate.getRootAsOrderUpdate(ByteBuffer.wrap(payload))));
          break;
        default:
          LOGGER.error("Unknown message type received");
          break;
      }
    }
  }

  public void registerListener(final Listener listener) {
    this.listeners.add(listener);
  }

  public void unregisterListener(final Listener listener) {
    this.listeners.remove(listener);
  }
}
