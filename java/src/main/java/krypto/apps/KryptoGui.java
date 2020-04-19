package krypto.apps;

import com.typesafe.config.Config;
import com.typesafe.config.ConfigFactory;
import krypto.instruments.InstrumentsClient;
import krypto.mktdata.Subscriber;
import krypto.pricing.PricingClient;
import krypto.ui.StartScreen;
import krypto.ui.UIDataCache;
import org.zeromq.ZMQ;

import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.util.Collections;

public class KryptoGui {
    public static void main(String[] args) {
        final var config = ConfigFactory.parseFile(new File(args[0])).resolve();

        final var context = ZMQ.context(1);

        System.err.println(config);

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

        EventQueue.invokeLater(() -> {
            try {
                UIManager.setLookAndFeel(
                        "com.jtattoo.plaf.noire.NoireLookAndFeel");
                UIManager.put("InternalFrame.titleFont",
                        new Font("Cambria Math", Font.BOLD, 12));
                final var frame = new StartScreen(uiDataCache);
                frame.setVisible(true);
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
    }
}
