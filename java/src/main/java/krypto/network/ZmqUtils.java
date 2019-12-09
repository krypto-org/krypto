package krypto.network;

import org.zeromq.ZMQ;

public class ZmqUtils {
    public static void sendEmptyFrame(final ZMQ.Socket socket,
                                      final int flags) {
        socket.send("", flags);
    }

    public static void sendMessageType(final ZMQ.Socket socket,
                                       final byte messageType,
                                       final int flags) {
        socket.send(new byte[] {messageType}, flags);
    }
}
