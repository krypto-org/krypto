package krypto.apps;

import com.github.weisj.darklaf.LafManager;
import com.github.weisj.darklaf.theme.DarculaTheme;
import com.typesafe.config.ConfigFactory;
import krypto.instruments.InstrumentsClient;
import krypto.mktdata.Subscriber;
import krypto.orders.OrderClient;
import krypto.pricing.PricingClient;
import krypto.ui.StartScreen;
import krypto.ui.UIDataCache;
import org.zeromq.ZMQ;

import java.awt.*;
import java.io.File;
import java.util.Collections;

public class KryptoGui {
    public static void main(String[] args) {
        final var config = ConfigFactory.parseFile(new File(args[0])).resolve();

        final var context = ZMQ.context(3);

        final var instrumentsClient = new InstrumentsClient(
                context, config.getString("services.instruments.client"));

        final var subscriber = new Subscriber(
                context,
                Collections.singletonList(config.getString(
                        "services.mktdata_gateway.backend.client")),
                true);
        subscriber.subscribe("");

        final var pricingClient = new PricingClient(
                context, Collections.singletonList(
                        config.getString("services.pricing.client")),
                true);
        pricingClient.subscribe("");

        final var uiDataCache = new UIDataCache(instrumentsClient);
        subscriber.registerListener(uiDataCache);
        pricingClient.registerListener(uiDataCache);

        subscriber.start();
        pricingClient.start();

        final OrderClient orderClient =
                new OrderClient(
                        context, config.getString("services.order_gateway.frontend.client"), 100);
        orderClient.start();

        EventQueue.invokeLater(() -> {
            try {
                LafManager.install(new DarculaTheme());
                final var frame = new StartScreen(uiDataCache, orderClient);
                frame.setVisible(true);
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
    }
}
