package krypto.network;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.zeromq.SocketType;
import org.zeromq.ZMQ;
import org.zeromq.ZMQException;

import java.util.List;

public abstract class BaseSubscriber implements SocketMonitor.Sniffer {

  private static Logger LOGGER = LogManager.getLogger(BaseSubscriber.class);

  private ZMQ.Socket monitorSocket;
  private SocketMonitor socketMonitor;

  private Thread subscriberThread;
  private Thread monitorThread;
  private String monitorEndpoint;
  private boolean monitor = true;

  protected ZMQ.Context context;
  protected List<String> addresses;
  protected ZMQ.Socket socket;
  protected volatile boolean terminated = false;

  public BaseSubscriber(
      final ZMQ.Context context, final List<String> addresses, final boolean monitor) {
    this.context = context;
    this.addresses = addresses;
    this.socket = context.socket(SocketType.SUB);
    this.monitor = monitor;
  }

  public void start() {
    if (monitor) {
      this.monitorEndpoint = SocketUtils.monitorEndpoint();
      this.monitorSocket = context.socket(SocketType.PAIR);
      this.monitorSocket.connect(this.monitorEndpoint);

      this.socketMonitor = new SocketMonitor(this);
      this.monitorThread = new Thread(this.socketMonitor);

      this.monitorThread.start();

      this.socket.monitor(this.monitorEndpoint, ZMQ.EVENT_ALL);
    }

    this.addresses.forEach(x -> socket.connect(x));
    this.subscriberThread = new Thread(this::process);
    this.subscriberThread.start();
    terminated = false;
  }

  public void stop() {
    terminated = true;
    this.addresses.forEach(x -> socket.disconnect(x));
    this.socket.close();

    if (subscriberThread.isAlive()) {
      try {
        subscriberThread.join(2000);
      } catch (InterruptedException | ZMQException e) {
        LOGGER.error(
            String.format(
                "Error while joining the subscriber thread. Error Message: %s", e.getMessage()));
      }
    }

    if (monitor) {
      this.socketMonitor.terminate();
      this.monitorSocket.disconnect(this.monitorEndpoint);
      this.monitorSocket.close();

      if (monitorThread.isAlive()) {
        try {
          monitorThread.join(2000);
        } catch (InterruptedException | ZMQException e) {
          LOGGER.error(
              String.format("Error while joining the subscriber thread - %s", e.getMessage()));
        }
      }
    }
  }

  public void subscribe(String topic) {
    LOGGER.debug("Subscribing to topic: {}", topic);
    this.socket.subscribe(topic.getBytes());
  }

  public void unsubscribe(String topic) {
    LOGGER.debug("Un-subscribing topic: {}", topic);
    this.socket.unsubscribe(topic.getBytes());
  }

  @Override
  public void onStatusUpdate(String address, SocketMonitor.EventType evt) {
    LOGGER.info("--- Monitor --- [{}] {}", address, evt);
  }

  @Override
  public ZMQ.Socket getMonitorSocket() {
    return this.monitorSocket;
  }

  protected abstract void process();
}
