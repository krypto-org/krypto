package krypto.testapps;

import krypto.Config;
import krypto.orders.OrderClient;
import krypto.serialization.OrderStatus;
import krypto.serialization.Side;
import krypto.serialization.TimeInForce;
import org.zeromq.ZMQ;

import java.io.IOException;

public class OrderClientApp {
  public static void main(String[] args) throws InterruptedException, IOException {
    final var config = new Config(args[0]);

    final ZMQ.Context context = ZMQ.context(1);
    final OrderClient client =
        new OrderClient(
            context, config.getStringNode("/services/order_gateway/frontend/client"), 100);

    System.out.println("Starting client");

    client.start();

    client.registerListener(
        orderUpdate ->
            System.out.println(
                orderUpdate.orderId() + ": " + OrderStatus.name(orderUpdate.status())));

    System.out.println("Sending order");

    final long securityId = 10200030018L;
    final double price = 7000.0;
    final double qty = 0.01;
    final var orderId = client.newOrder("SIM", securityId, price, qty, Side.BUY, TimeInForce.DAY);
    System.out.println("Waiting ..... ");
    Thread.sleep(2000);
    System.out.println("Cancel order");
    client.cancelOrder("SIM", orderId);
    Thread.sleep(2000);

    client.stop();
  }
}
