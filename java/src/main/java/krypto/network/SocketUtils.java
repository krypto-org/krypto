package krypto.network;

import org.zeromq.ZMQ;

import java.util.UUID;

public class SocketUtils {
    public static void sendEmptyFrame(final ZMQ.Socket socket,
                                      final int flags) {
        socket.send("", flags);
    }

    public static void sendMessageType(final ZMQ.Socket socket,
                                       final byte messageType,
                                       final int flags) {
        socket.send(new byte[] {messageType}, flags);
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
}
