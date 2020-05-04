package krypto.network;

import org.zeromq.ZMQ;

import java.util.UUID;

public class SocketUtils {
    public static void connect(final ZMQ.Socket socket,
                               final String endpoint, final String identity) {
        socket.setIdentity(identity.getBytes());
        socket.setLinger(0);
        socket.connect(endpoint);
    }

    public static void sendEmptyFrame(final ZMQ.Socket socket, final int flags) {
        socket.send("", flags);
    }

    public static void sendMessageType(
            final ZMQ.Socket socket, final byte messageType, final int flags) {
        socket.send(new byte[] {messageType}, flags);
    }

    public static byte receiveMessageType(final ZMQ.Socket socket) {
        return socket.recv()[0];
    }

    public static String monitorEndpoint() {
        return inprocEndpoint("events", true);
    }

    public static String inprocEndpoint(String name, boolean unique) {
        String rv = String.format("inproc://%s", name);
        if (unique) {
            rv = String.format("%s-%s", rv, UUID.randomUUID());
        }
        return rv;
    }

    public static String generateIdentity(final String prefix) {
        final var ts = System.currentTimeMillis();
        return String.format("%s-%d", prefix, ts);
    }
}
