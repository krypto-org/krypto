package krypto.pricing;

import krypto.network.BaseSubscriber;
import krypto.network.MessageType;
import krypto.serialization.*;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.zeromq.ZMQ;

import java.nio.ByteBuffer;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class PricingClient extends BaseSubscriber {

  private static final Logger LOGGER = LogManager.getLogger(PricingClient.class);

  public interface Listener {
    void onTheoreticalSnapshot(final TheoreticalSnapshot snapshot);
  }

  private final Set<Listener> listeners = new HashSet<>();

  public PricingClient(ZMQ.Context context, List<String> addresses, boolean monitor) {
    super(context, addresses, monitor);
  }

  @Override
  protected void process() {
    super.addresses.forEach(
        x -> LOGGER.info(String.format("Connecting to pricing server @ %s", x)));

    try {
      while (!super.terminated) {
        final String topic = super.socket.recvStr();

        final byte messageType = MessageType.MsgTypeNames.get(topic.substring(0, 2));

        if (messageType == MessageType.THEO) {
          this.listeners.forEach(
              listener ->
                  listener.onTheoreticalSnapshot(
                      TheoreticalSnapshot.getRootAsTheoreticalSnapshot(
                          ByteBuffer.wrap(socket.recv()))));
        } else {
          LOGGER.error("Unknown message type received");
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
