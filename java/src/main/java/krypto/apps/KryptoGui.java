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
        final Config config = ConfigFactory.parseFile(new File("config.hocon")).resolve();
        final ZMQ.Context context = ZMQ.context(1);

        final InstrumentsClient instrumentsClient = new InstrumentsClient(context, "tcp://localhost:12000");

        Subscriber subscriber = new Subscriber(
                context,
                Collections.singletonList("tcp://localhost:12021"),
                true);
        subscriber.subscribe("");

        PricingClient pricingClient = new PricingClient(
                context, Collections.singletonList("tcp://localhost:12030"),
                true);
        pricingClient.subscribe("");

        final UIDataCache uiDataCache = new UIDataCache(instrumentsClient);
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
                StartScreen frame = new StartScreen(uiDataCache);
                frame.setVisible(true);
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
    }
}
