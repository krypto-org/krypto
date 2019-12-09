package krypto.examples;

import krypto.mktdata.Subscriber;
import krypto.serialization.Heartbeat;
import krypto.serialization.OrderUpdate;
import krypto.serialization.Quote;
import krypto.serialization.Trade;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.zeromq.ZMQ;

import java.util.Arrays;
import java.util.Collections;

public class MktdataSubscriberPrinter {
    private static Logger LOGGER = LogManager.getLogger(MktdataSubscriberPrinter.class);


    public static void main(String[] args) {
        final ZMQ.Context context = ZMQ.context(1);

        Subscriber subscriber = new Subscriber(
                context,
                Collections.singletonList("tcp://192.168.1.12:12021"),
                true);
        subscriber.subscribe("");
        subscriber.start();

        subscriber.registerListener(new Subscriber.Listener() {
            @Override
            public void onHeartbeat(Heartbeat hb) {
                LOGGER.info("Heartbeat for {}", hb.securityId());
            }

            @Override
            public void onQuote(Quote quote) {
                LOGGER.info("Quote [{}]: {} @ {} | {} @ {}",
                        quote.securityId(), quote.bidQuantity(), quote.bid(),
                        quote.askQuantity(), quote.ask());
            }

            @Override
            public void onTrade(Trade trade) {
                LOGGER.info("Trade [{}]: {} @ {}",
                        trade.securityId(), trade.quantity(), trade.price());

            }

            @Override
            public void onOrderUpdate(OrderUpdate ou) {
                // ignore
            }
        });
    }
}
