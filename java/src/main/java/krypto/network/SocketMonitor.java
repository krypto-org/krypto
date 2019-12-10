package krypto.network;

import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.zeromq.ZMQ;
import zmq.ZError;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class SocketMonitor implements Runnable {

    private static Logger logger = LogManager.getLogger(SocketMonitor.class);

    public enum EventType {
        LISTENING,
        ACCEPTED,
        CONNECTED,
        ACCEPT_FAILED,
        BIND_FAILED,
        CLOSE_FAILED,
        CLOSED,
        CONNECT_DELAYED,
        DISCONNECTED,
        CONNECT_RETRIED,
        MONITOR_STOPPED,
        HANDSHAKE_PROTOCOL
    }

    private static Map<Integer, EventType> ZMQ_EVENT_REFERENCE;

    static {
        ZMQ_EVENT_REFERENCE = new ConcurrentHashMap<>();
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_LISTENING, EventType.LISTENING);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_ACCEPTED, EventType.ACCEPTED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_CONNECTED, EventType.CONNECTED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_ACCEPT_FAILED, EventType.ACCEPT_FAILED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_BIND_FAILED, EventType.BIND_FAILED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_CLOSE_FAILED, EventType.CLOSE_FAILED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_CLOSED, EventType.CLOSED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_CONNECT_DELAYED, EventType.CONNECT_DELAYED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_DISCONNECTED, EventType.DISCONNECTED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_CONNECT_RETRIED, EventType.CONNECT_RETRIED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_MONITOR_STOPPED, EventType.MONITOR_STOPPED);
        ZMQ_EVENT_REFERENCE.put(ZMQ.EVENT_HANDSHAKE_PROTOCOL, EventType.HANDSHAKE_PROTOCOL);
    }


    public interface Sniffer {
        ZMQ.Socket getMonitorSocket();
        void onStatusUpdate(String address, EventType evt);
    }

    private volatile boolean terminated = false;

    private Sniffer sniffer;

    SocketMonitor(final Sniffer sniffer) {
        this.sniffer = sniffer;
    }

    @Override
    public void run() {
        while (!terminated) {
            try {
                ZMQ.Event event = ZMQ.Event.recv(sniffer.getMonitorSocket());
                if (event == null && sniffer.getMonitorSocket().errno() == ZError.ETERM) {
                    break;
                }
                sniffer.onStatusUpdate(
                        event.getAddress(),
                        ZMQ_EVENT_REFERENCE.get(
                                event.getEvent()));
            } catch (Exception e) {
                logger.error("Received an Error in Socket Monitor: {}", e.getMessage());
            }
        }
    }

    public void terminate() {
        this.terminated = false;
    }
}
